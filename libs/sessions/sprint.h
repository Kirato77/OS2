// sprint.h
#ifndef SPRINT_H
#define SPRINT_H

#include <stdio.h>
#include <unistd.h>

void simulateSprint() {
    printf("Simulation de la Course Sprint en cours...\n");
    // logique
    sleep(5);  // Simulation d'une session de 5 secondes
    printf("Fin de la Course Sprint.\n");
}

#endif