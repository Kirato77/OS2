// race.h
#ifndef RACE_H
#define RACE_H

#include <stdio.h>
#include <unistd.h>

void simulateRace() {
    printf("Simulation de la Course en cours...\n");
    // logique
    sleep(5);  // Simulation d'une session de 5 secondes
    printf("Fin de la Course.\n");
}

#endif