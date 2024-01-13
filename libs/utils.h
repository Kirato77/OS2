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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#define MAX_LINE_SIZE 1024
#define MAX_FIELD_SIZE 256

void getPilots() {
    FILE *file = fopen("data/Liste des pilotes.csv", "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_SIZE];
    char *token;

    iconv_t conversion = iconv_open("UTF-8", "ISO-8859-1");

    if (conversion == (iconv_t)-1) {
        perror("Erreur lors de l'initialisation de la conversion de caractères");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        token = strtok(line, ",");

        while (token != NULL) {
            size_t in_size = strlen(token);
            size_t out_size = MAX_FIELD_SIZE;
            char output[MAX_FIELD_SIZE];

            char *output_ptr = output;  // Utiliser un pointeur de type char *

            if (iconv(conversion, &token, &in_size, &output_ptr, &out_size) == (size_t)-1) {
                perror("Erreur lors de la conversion de caractères");
                exit(EXIT_FAILURE);
            }

            *output_ptr = '\0';  // Assurez-vous que la chaîne de sortie est terminée correctement
            printf("%s\t", output);

            token = strtok(NULL, ",");
        }

        printf("\n");
    }

    iconv_close(conversion);
    fclose(file);
}


#endif