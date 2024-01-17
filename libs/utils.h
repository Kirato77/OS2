// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define MAX_LINE_LENGTH 256
#define MAX_FIELDS 20

float getRandomTime(int trackLength) {
    int vmax = 64;
    int vmin = 44;

    float tmax = ((trackLength * 1000.0) / vmin) * 1000.0;
    float tmin = ((trackLength * 1000.0) / vmax) * 1000.0;

    // Initialisation of the seed
    srand(time(NULL));

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

#define NUM_PILOTS 10
#define NUM_SECTORS 4
// Structure pour stocker les informations d'un pilote
typedef struct {
    int number;
    float sectorTimes[NUM_SECTORS];
    float bestLapTime;
    float totalTime;
    int stand;
    int out;
} PilotStats;

// Structure pour la mémoire partagée
typedef struct {
    PilotStats pilots_stats[NUM_PILOTS];
} SharedMemory;

// Fonction pour afficher le tableau des résultats
void displayResults(SharedMemory *sharedMemory) {
    printf("\nTableau des résultats :\n");
    printf("---------------------------------------------------------\n");
    printf("| Pilote | Secteur 1 | Secteur 2 | Secteur 3 | Meilleur Tour | Total |\n");
    printf("---------------------------------------------------------\n");

    for (int i = 0; i < NUM_PILOTS; ++i) {
        printf("|   %2d   |", sharedMemory->pilots_stats[i].number);

        for (int sector = 0; sector < NUM_SECTORS; ++sector) {
            printf("   %.2f   |", sharedMemory->pilots_stats[i].sectorTimes[sector]);
        }

        printf("   %.2f   |", sharedMemory->pilots_stats[i].bestLapTime);
        printf("   %.2f   |\n", sharedMemory->pilots_stats[i].totalTime);
        printf("---------------------------------------------------------\n");
    }

    // Afficher les meilleurs temps par secteur parmi tous les pilotes
    printf("\nMeilleurs temps par secteur :\n");
    for (int sector = 0; sector < NUM_SECTORS; ++sector) {
        float bestSectorTime = FLT_MAX;
        int bestPilotIndex = -1;

        for (int i = 0; i < NUM_PILOTS; ++i) {
            if (sharedMemory->pilots_stats[i].sectorTimes[sector] < bestSectorTime) {
                bestSectorTime = sharedMemory->pilots_stats[i].sectorTimes[sector];
                bestPilotIndex = i;
            }
        }

        printf("Meilleur temps Secteur %d : Pilote %d avec %.2f secondes\n", sector + 1,
               sharedMemory->pilots_stats[bestPilotIndex].number, bestSectorTime);
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
        fprintf(file, "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                sharedMemory->pilots_stats[i].number,
                sharedMemory->pilots_stats[i].sectorTimes[0],
                sharedMemory->pilots_stats[i].sectorTimes[1],
                sharedMemory->pilots_stats[i].sectorTimes[2],
                sharedMemory->pilots_stats[i].bestLapTime,
                sharedMemory->pilots_stats[i].totalTime);
    }

    fclose(file);
}

void genererTempsAleatoires(int min, int max, float temps[4]) {


    // Générer des temps aléatoires pour chaque secteur

    temps[0] = rand() % (max - min + 1) + min;

    temps[1] =  rand() % (max - min + 1) + min;
    temps[2] = rand() % (max - min + 1) + min;

    // Calculer le temps total du tour
    temps[3] = temps[0] + temps[1] + temps[2]; // Tour
}

float *drive(){

    //initialisation des meilleurs temps à 0
    float *meilleursTemps = (float *)malloc(4 * sizeof(float));
    for (int i = 0; i < 4; i++) {
        meilleursTemps[i] = 0;
    }

    //génerére le temps de ce tour dans un tableau
    float temps[4] = {0, 0, 0, 0};
    genererTempsAleatoires(25000, 45000, temps);

    //sauver les temps si ils sont meilleur que les précedents
    if (meilleursTemps[0] == 0 || meilleursTemps[0] > temps[0]){
        meilleursTemps[0] = temps[0];
    }
    if (meilleursTemps[1] == 0 || meilleursTemps[1] > temps[1]){
        meilleursTemps[1] = temps[1];
    }
    if (meilleursTemps[2] == 0 || meilleursTemps[2] > temps[2]){
        meilleursTemps[2] = temps[2];
    }
    if (meilleursTemps[3] == 0 || meilleursTemps[3] > temps[3]){
        meilleursTemps[3] = temps[3];
    }

    return meilleursTemps;
}

int compNum(const void *a, const void *b);
void sortpilots_statsByNum(struct PilotStats *resultat, int nombreDePilotes) {
    qsort(resultat, nombreDePilotes, sizeof(struct PilotStats), compNum);
}

#endif