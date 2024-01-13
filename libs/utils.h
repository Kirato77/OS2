#include <stdio.h>
#include <stdlib.h>
#include <time.h>

max = 25
min = 5

int main() {
    // Initialisation of the seed
    srand(time(NULL));

    // Division between max and min
    int nombreDansUnePlage = nombreAleatoire % (max - min + 1) + min;

    printf("Nombre aléatoire : %d\n", nombreDansUnePlage);

    return 0;
}
