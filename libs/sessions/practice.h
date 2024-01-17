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

    for (int k = 1; k <= 3; k++) {
        // Reinitialiser la m�moire partag�e
        initializeSharedMemory(attachedMemory);

        // Cr�er un processus fils pour afficher les r�sultats toutes les 5 secondes
        pid_t pid_display = fork();
        if (pid_display == -1) {
            perror("Erreur lors de la cr�ation du processus fils pour l'affichage");
            exit(EXIT_FAILURE);
        }
        if (pid_display == 0) {
            // Code ex�cut� par le processus fils pour l'affichage
            while (1) {
                // Effacer la console
                system("clear"); // Utiliser la commande clear pour Linux ou macOS
                // Afficher les r�sultats
                displayResults(attachedMemory);
                // Attendre 5 secondes
                sleep(5); // Utiliser la fonction sleep pour ralentir le programme
            }
        }

        // Boucle pour cr�er un processus par voiture
        for (int i = 0; i < numPilots; ++i) {
            pid_t pid = fork();

            if (pid == -1) {
                perror("Erreur lors de la cr�ation du processus fils");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                // Code ex�cut� par le processus fils
                srand(time(NULL) ^ getpid());
                for (int tour = 1; tour <= nbToursP(5); ++tour) {
                    float lapTime = 0;

                    // G�n�rer trois temps al�atoires et mettre � jour la SharedMemory
                    for (int j = 0; j < 3; ++j) {
                        float randomTime = getRandomTime(5);
                        attachedMemory->pilots[i].totalTime += randomTime; // Incrementer le temps total
                        lapTime += randomTime;

                        // V�rifier et mettre � jour la SharedMemory si n�cessaire pour les temps par secteur
                        if (attachedMemory->pilots[i].sectorTimes[j] == 0 || randomTime < attachedMemory->pilots[i].sectorTimes[j]) {
                            attachedMemory->pilots[i].sectorTimes[j] = randomTime;
                        }

                        // sleep(1); // Utiliser la fonction sleep pour ralentir le programme
                    }
                    // V�rifier et mettre � jour la SharedMemory si n�cessaire pour le meilleur temps
                    if (attachedMemory->pilots[i].bestLapTime == 0 || lapTime < attachedMemory->pilots[i].bestLapTime) {
                        attachedMemory->pilots[i].bestLapTime = lapTime;
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

        // Terminer le processus fils pour l'affichage
        kill(pid_display, SIGTERM);
        displayResults(attachedMemory);

        // Afficher un message entre chaque boucle
        if (k < 3) {
            printf("\nAppuyez sur Entr�e pour passer � la session P%d\n", k + 1);
            getchar(); // Attendre que l'utilisateur appuie sur Entr�e
        }
    }

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