// practice.h
#ifndef PRACTICE_H
#define PRACTICE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>

#include "../utils.h"
#include "../constants.h"
#include "../utils/customJSON.h"


void simulateFreePractice(SharedMemory *sharedMemory) {

    float trackLength = getTrackByIndex("./data/tracks.csv", getLastTrackValue(readJSONFile("./data/data.json"))+1).Size;

    // Initialiser la mémoire partagée
    initializeSharedMemory(sharedMemory);

    // Créer une clé IPC pour la mémoire partagée
    key_t key = ftok(".", 's');
    if (key == -1) {
        perror("Erreur lors de la création de la clé IPC");
        exit(EXIT_FAILURE);
    }

    // Créer la mémoire partagée
    int shmid = shmget(key, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Erreur lors de la création de la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    // Attacher la mémoire partagée
    SharedMemory *attachedMemory = shmat(shmid, NULL, 0);
    if (attachedMemory == (void *)-1) {
        perror("Erreur lors de l'attachement de la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    // Copier le contenu de sharedMemory dans la mémoire partagée
    memcpy(attachedMemory, sharedMemory, sizeof(SharedMemory)); // Utiliser la fonction memcpy pour copier les données

    // Nombre de pilotes
    int numPilots = sizeof(attachedMemory->pilots) / sizeof(attachedMemory->pilots[0]);

    for (int k = 1; k <= 3; k++) {
        // Reinitialiser la mémoire partagée
        initializeSharedMemory(attachedMemory);

        int currentProcessNumber = 1;

        // Boucle pour créer un processus par voiture
        for (int i = 0; i < numPilots; ++i) {
            pid_t pid = fork();
            if (pid == -1) {
                perror("Erreur lors de la création du processus fils");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                // Code exécuté par le processus fils
                srand(time(NULL) ^ getpid());
                for (int tour = 1; tour <= nbToursP(trackLength); ++tour) {

                    float lapTime = 0;

                    // Générer trois temps aléatoires et mettre à jour la SharedMemory
                    for (int j = 0; j < 3; ++j) {
                        float randomTime = getRandomTime(trackLength);
                        attachedMemory->pilots[i].totalTime += randomTime; // Incrementer le temps total
                        lapTime += randomTime;

                        // Vérifier et mettre à jour la SharedMemory si nécessaire pour les temps par secteur
                        if (attachedMemory->pilots[i].sectorTimes[j] == 0 || randomTime < attachedMemory->pilots[i].sectorTimes[j]) {
                            attachedMemory->pilots[i].sectorTimes[j] = randomTime;
                        }
                    }
                    // Vérifier et mettre à jour la SharedMemory si nécessaire pour le meilleur temps
                    if (attachedMemory->pilots[i].bestLapTime == 0 || lapTime < attachedMemory->pilots[i].bestLapTime) {
                        attachedMemory->pilots[i].bestLapTime = lapTime;
                    }
                    usleep((int)(lapTime/TIME_MULTIPLIER*1000000));
                    if (currentProcessNumber == 1) {
                        system("clear");
                        // Sort the pilots array based on totalTime

                        displayResults(attachedMemory, 0, comparePilotsBestLap);
                    }
                }

                // Terminer le processus fils
                exit(EXIT_SUCCESS);
            }
            currentProcessNumber++;
        }

        while(wait(NULL) > 0);

        // Concatenate strings and format numbers
        char fileName[100];
        snprintf(fileName, sizeof(fileName), "Track%d_P%d", getLastTrackValue(readJSONFile(DATA_JSON_FILE)) + 1, k);
        saveResults(attachedMemory, DATA_STATS_DIR, fileName);

        // Afficher un message entre chaque boucle
        if (k < 3) {
            printf("\nAppuyez sur Entrée pour passer à la session P%d\n", k + 1);
            getchar(); // Attendre que l'utilisateur appuie sur Entrée
        }
    }

    // Détacher la mémoire partagée dans le processus parent
    if (shmdt(attachedMemory) == -1) {
        perror("Erreur lors du détachement de la mémoire partagée dans le processus parent");
        exit(EXIT_FAILURE);
    }

    // Supprimer la mémoire partagée
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("Erreur lors de la suppression de la mémoire partagée");
        exit(EXIT_FAILURE);
    }
}

#endif