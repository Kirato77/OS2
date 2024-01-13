// random_generator.h
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getRandom(int longueur) {

    int vmax = 64;
    int vmin = 44;

    int tmax = (longueur*1000)/vmin;
    int tmin = (longueur*1000)/vmax;

    // Initialisation of the seed
    srand(time(NULL));

    // Generate random number between 0 and RAND_MAX
    int randomNumber = rand();

    // Division between max and min
    int number = randomNumber % (tmax - tmin + 1) + tmin;

    return number;
}





#endif