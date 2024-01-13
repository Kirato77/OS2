// utils.h
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float getRandom(int circuitLength) {
    int vmax = 64;
    int vmin = 44;

    float tmax = ((circuitLength * 1000.0) / vmin) * 1000.0;
    float tmin = ((circuitLength * 1000.0) / vmax) * 1000.0;

    // Initialisation of the seed
    srand(time(NULL));

    // Generate random number between 0 and RAND_MAX
    int randomNumber = rand();

    // Division between max and min with decimals
    int number = randomNumber % (int)(tmax - tmin + 1) + (int)tmin;
    float result = (float)number / 3000;

    return result;
}




#endif