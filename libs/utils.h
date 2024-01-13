// random_generator.h
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getRandom(int min, int max) {

    // Initialisation of the seed
    srand(time(NULL));

    // Generate random number between 0 and RAND_MAX
    int randomNumber = rand();

    // Division between max and min
    int number = randomNumber % (max - min + 1) + min;

    return number;
}





#endif