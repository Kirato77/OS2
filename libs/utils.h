// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <unistd.h>

// For mkdir function on Unix-like systems
#include <sys/stat.h>

#include "utils/customCSV.h"

#define MAX_LINE_LENGTH 256
#define MAX_FIELDS 20

float getRandomTime(int trackLength) {
    int vmax = 64;
    int vmin = 44;

    float tmax = ((trackLength * 1000.0) / vmin) * 1000.0;
    float tmin = ((trackLength * 1000.0) / vmax) * 1000.0;

    // Generate random number between 0 and RAND_MAX
    int randomNumber = rand();

    // Division between max and min with decimals
    int number = randomNumber % (int) (tmax - tmin + 1) + (int) tmin;
    float result = (float) number / 3000;

    return result;
}

int nbToursP(int trackLength) {
    int temps = 0;
    int x = 0;
    while (temps < 3600) {
        int n = 3 * getRandomTime(trackLength);
        temps += n;
        x += 1;
    }
    return x;
}

#define NUM_PILOTS 20
#define NUM_SECTORS 3

// Structure pour la mémoire partagée
typedef struct {
    struct Pilot pilots[20];
} SharedMemory;

// Fonction pour initialiser les valeurs des pilotes dans la structure SharedMemory
void initializeSharedMemory(SharedMemory *sharedMemory) {
    int rowIndex = 0;
    readPilotsCSVFile("./data/pilots.csv", sharedMemory->pilots, &rowIndex);

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 3; ++j) {
            sharedMemory->pilots[i].sectorTimes[j] = 0.0f;
        }
        sharedMemory->pilots[i].bestLapTime = 0.0f;
        sharedMemory->pilots[i].totalTime = 0.0f;
        sharedMemory->pilots[i].pit = 0;
        sharedMemory->pilots[i].out = 0;
    }
}

// Comparison functions for qsort
int comparePilotsTotalTime(const void *a, const void *b) {
    // Assuming totalTime is an integer field
    return ((struct Pilot *)a)->totalTime - ((struct Pilot *)b)->totalTime;
}
int comparePilotsBestLap(const void *a, const void *b) {
    // Assuming bestLapTime is a float
    float timeA = ((struct Pilot *)a)->bestLapTime;
    float timeB = ((struct Pilot *)b)->bestLapTime;

    // Compare bestLapTime values
    if (timeA < timeB) {
        return -1;
    }
    if (timeA > timeB) {
        return 1;
    }
    return 0; // bestLapTime values are equal
}

// Fonction pour afficher le tableau des résultats
void displayResults(SharedMemory *sharedMemory, const int showTotalTime, int (*operation)(const void *, const void *)) {

    qsort(sharedMemory->pilots, sizeof(sharedMemory->pilots) / sizeof(sharedMemory->pilots[0]), sizeof(sharedMemory->pilots[0]), *operation);

    printf("\n\033[1;35mTableau des résultats :\033[0m\n");
    const char *bar = "------------------------------------------------------------------------------------------------------------------------------------------\033[0m\n";
    if (showTotalTime == 0) {
        bar = "-----------------------------------------------------------------------------------------------------------------------------\033[0m\n";
        printf("\033[1;31m-----------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-5s | %-5s | %-10s | %-10s | %-10s | %-10s | %-13s | %-10s | %-10s | %-10s |\n","nb" ,"Numéro", "Pilote", "Secteur 1", "Secteur 2", "Secteur 3", "Meilleur Tour", "DIFF", "PIT", "OUT");
        printf("%s", bar);

    } else {
        printf("\033[1;31m------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-5s | %-5s | %-10s | %-10s | %-10s | %-10s | %-13s | %-10s | %-10s | %-10s | %-10s |\n","nb" ,"Numéro", "Pilote", "Secteur 1", "Secteur 2", "Secteur 3", "Meilleur Tour", "Total", "DIFF", "PIT", "OUT");
        printf("%s", bar);
    }

    for (int i = 0; i < NUM_PILOTS; ++i) {
        printf("\033[1;33m| %-5d |", i+1);
        printf(" %-5d  |", sharedMemory->pilots[i].Num);
        printf(" %-10s", sharedMemory->pilots[i].Name);
        if(strstr(sharedMemory->pilots[i].Name, "é") != NULL) { // lmao C
            printf(" ");
        }
        printf(" |");

        for (int sector = 0; sector < NUM_SECTORS; ++sector) {
            printf(" %-10.2f |", sharedMemory->pilots[i].sectorTimes[sector]);
        }
        float diff = 0;
        if (i != 0){
            if (showTotalTime == 1) {
                diff = sharedMemory->pilots[i-1].totalTime - sharedMemory->pilots[i].totalTime;
            } else {
                diff = sharedMemory->pilots[i-1].bestLapTime - sharedMemory->pilots[i].bestLapTime;
            }
        }
        else {
            diff = 0;
        }
        printf(" %-13.2f |", sharedMemory->pilots[i].bestLapTime);
        if (showTotalTime == 1) {
            printf(" %-10.2f |", sharedMemory->pilots[i].totalTime);
        }
        printf(" %-10.2f |", diff);

        // Print "PIT" if .pit is 1, nothing otherwise
        printf(" %-10s |", sharedMemory->pilots[i].pit == 1 ? "PIT" : "");
        // Print "OUT" if .out is 1, nothing otherwise
        printf(" %-10s |\n", sharedMemory->pilots[i].out == 1 ? "OUT" : "");

        printf("%s", bar);
    }

    // Afficher les meilleurs temps par secteur parmi tous les pilotes
    printf("\nMeilleurs temps par secteur :\n");
    for (int sector = 0; sector < NUM_SECTORS; ++sector) {
        float bestSectorTime = FLT_MAX;
        int bestPilotIndex = -1;

        for (int i = 0; i < NUM_PILOTS; ++i) {
            if (sharedMemory->pilots[i].sectorTimes[sector] < bestSectorTime) {
                bestSectorTime = sharedMemory->pilots[i].sectorTimes[sector];
                bestPilotIndex = i;
            }
        }

        printf("Meilleur temps Secteur %d : Pilote %s avec %.2f secondes\n", sector + 1,
               sharedMemory->pilots[bestPilotIndex].Name, bestSectorTime);
    }
}


#define MAX_PATH_LENGTH 256

// Function to save the results to a CSV file
void saveResults(SharedMemory *sharedMemory,  const char *directoryName, const char *filename) {
    // Create the "westats" directory if it doesn't exist
    if (mkdir(directoryName, 0777) == -1) {
        // Directory already exists or there was an error
    }

    // Create the CSV file path
    char filePath[MAX_PATH_LENGTH];
    snprintf(filePath, sizeof(filePath), "%s/%s.csv", directoryName, filename);

    // Open the file for writing
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    qsort(sharedMemory->pilots, sizeof(sharedMemory->pilots) / sizeof(sharedMemory->pilots[0]), sizeof(sharedMemory->pilots[0]), comparePilotsBestLap);

    // Write the CSV header
    fprintf(file, "Num,Pilote,Secteur 1,Secteur 2,Secteur 3,Meilleur Tour\n");

    // Write the race results to the CSV file
    for (int i = 0; i < NUM_PILOTS; ++i) {
        fprintf(file, "%d,%s,%.2f,%.2f,%.2f,%.2f\n",
                sharedMemory->pilots[i].Num, sharedMemory->pilots[i].Name,
                sharedMemory->pilots[i].sectorTimes[0], sharedMemory->pilots[i].sectorTimes[1],
                sharedMemory->pilots[i].sectorTimes[2], sharedMemory->pilots[i].bestLapTime);
    }

    // Close the file
    fclose(file);

    printf("Results saved to %s\n", filePath);
}

#endif