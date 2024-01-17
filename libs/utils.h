// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

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

// Fonction pour afficher le tableau des résultats
void displayResults(SharedMemory *sharedMemory) {
    printf("\n\033[1;35mTableau des résultats :\033[0m\n");
    printf("\033[1;31m------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-5s | %-10s | %-10s | %-10s | %-10s | %-13s | %-10s | %-10s | %-10s | %-10s |\n","nb" ,"Numéro", "Pilote", "Secteur 1", "Secteur 2", "Secteur 3", "Meilleur Tour", "Total", "DIFF", "PIT", "OUT");
    printf("------------------------------------------------------------------------------------------------------------------------------------------\033[0m\n");

    for (int i = 0; i < NUM_PILOTS; ++i) {
        printf("\033[1;33m| %-5d |", i+1);
        printf(" %-5d  |", sharedMemory->pilots[i].Num);
        printf(" %-10s |", sharedMemory->pilots[i].Name);

        for (int sector = 0; sector < NUM_SECTORS; ++sector) {
            printf(" %-10.2f |", sharedMemory->pilots[i].sectorTimes[sector]);
        }
        float diff = 0;
        if (i != 0){
            diff = sharedMemory->pilots[i-1].totalTime - sharedMemory->pilots[i].totalTime;
        }
        else {
            diff = 0;
        }
        printf(" %-13.2f |", sharedMemory->pilots[i].bestLapTime);
        printf(" %-10.2f |", sharedMemory->pilots[i].totalTime);
        printf(" %-10.2f |", diff);
        printf(" %-10.d |", sharedMemory->pilots[i].pit);
        printf(" %-10.d |\n", sharedMemory->pilots[i].out);
        printf("------------------------------------------------------------------------------------------------------------------------------------------\033[0m\n");
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

// Fonction pour sauvegarder les résultats dans un fichier CSV
void saveResults(int index, SharedMemory *sharedMemory) {
    char filename[50];
    snprintf(filename, sizeof(filename), "/data/westats/%d.csv", index);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur d'ouverture du fichier pour sauvegarde");
        exit(EXIT_FAILURE);
    }

    // En-têtes du fichier CSV
    fprintf(file, "Pilote,Secteur 1,Secteur 2,Secteur 3,Meilleur Tour,Total\n");

    // Écriture des données dans le fichier
    for (int i = 0; i < NUM_PILOTS; ++i) {
        fprintf(file, "%s,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                sharedMemory->pilots[i].Name,
                sharedMemory->pilots[i].sectorTimes[0],
                sharedMemory->pilots[i].sectorTimes[1],
                sharedMemory->pilots[i].sectorTimes[2],
                sharedMemory->pilots[i].bestLapTime,
                sharedMemory->pilots[i].totalTime);
    }

    fclose(file);
}

#endif