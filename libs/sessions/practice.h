// practice.h
#ifndef PRACTICE_H
#define PRACTICE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "../utils.h"
#include "../utils/customJSON.h"

void simulateFreePractice(SharedMemory *sharedMemory) {
    // Initialiser la m�moire partag�e
    initializeSharedMemory(sharedMemory);

    // Cr�er une cl� IPC pour la m�moire partag�e
    key_t key = ftok(".", 's');
    if (key == -1) {
        perror("Erreur lors de la cr�ation de la cl� IPC");
        exit(EXIT_FAILURE);
    }

    // Cr�er la m�moire partag�e
    int shmid = shmget(key, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Erreur lors de la cr�ation de la m�moire partag�e");
        exit(EXIT_FAILURE);
    }

    // Attacher la m�moire partag�e
    SharedMemory *attachedMemory = shmat(shmid, NULL, 0);
    if (attachedMemory == (void *)-1) {
        perror("Erreur lors de l'attachement de la m�moire partag�e");
        exit(EXIT_FAILURE);
    }

    // Copier le contenu de sharedMemory dans la m�moire partag�e
    memcpy(attachedMemory, sharedMemory, sizeof(SharedMemory)); // Utiliser la fonction memcpy pour copier les donn�es

    // Nombre de pilotes
    int numPilots = sizeof(attachedMemory->pilots) / sizeof(attachedMemory->pilots[0]);

    // Boucle pour cr�er un processus par voiture
    for (int i = 0; i < numPilots; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Erreur lors de la cr�ation du processus fils");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            srand(time(NULL) ^ (getpid()<<16));
            // Code ex�cut� par le processus fils

            for (int tour = 1; tour <= nbToursP(5); ++tour) {
                // G�n�rer trois temps al�atoires et mettre � jour la SharedMemory
                for (int j = 0; j < 3; ++j) {
                    float randomTime = getRandomTime(5);

                    // V�rifier et mettre � jour la SharedMemory si n�cessaire
                    if (attachedMemory->pilots[i].sectorTimes[j] == 0 || randomTime < attachedMemory->pilots[i].sectorTimes[j]) {
                        attachedMemory->pilots[i].sectorTimes[j] = randomTime;
                    }
                }
            }

            // Terminer le processus fils
            exit(EXIT_SUCCESS);
        }
    }

    // Attendre que tous les processus fils se terminent
    for (int i = 0; i < numPilots; ++i) {
        wait(NULL);
    }

    displayResults(attachedMemory);

    // D�tacher la m�moire partag�e dans le processus parent
    if (shmdt(attachedMemory) == -1) {
        perror("Erreur lors du d�tachement de la m�moire partag�e dans le processus parent");
        exit(EXIT_FAILURE);
    }

    // Supprimer la m�moire partag�e
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("Erreur lors de la suppression de la m�moire partag�e");
        exit(EXIT_FAILURE);
    }
}

#endif