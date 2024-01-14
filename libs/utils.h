// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_FIELDS 20

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
    int number = randomNumber % (int) (tmax - tmin + 1) + (int) tmin;
    float result = (float) number / 3000;

    return result;
}

int nbToursP(int longueur) {
    int temps = 0;
    int x = 0;
    while (temps < 3600) {
        int n = 3 * getRandom(longueur);
        temps += n;
        x += 1;
    }
    return x;
}


typedef struct {
    int nb;
    int num;
    char prenom[50];
    char nom[50];
    char team[50];
    char naissance[11];
    char nationalite[50];
} F1Driver;

void parseCSVLine(const char *line, F1Driver *driver) {
    // Utilisation de sscanf pour extraire les valeurs du CSV
    sscanf(line, "%d,%d,%49[^,],%49[^,],%49[^,],%10[^,],%49[^,]",
           &driver->nb, &driver->num, driver->prenom, driver->nom, driver->team,
           driver->naissance, driver->nationalite);
}

int getPilots(int num) {
    num = num - 1;
    // Ouverture du fichier CSV en mode lecture
    FILE *file = fopen("data/pilots.csv", "r");

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    F1Driver drivers[MAX_FIELDS];

    // Ignorer la première ligne
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Erreur lors de la lecture de la première ligne.\n");
        fclose(file);
        return 1;
    }

    int i = 0;

    // Lecture du fichier ligne par ligne
    while (fgets(line, sizeof(line), file) != NULL && i < MAX_FIELDS) {
        // Supprimer le caractère de nouvelle ligne
        line[strcspn(line, "\n")] = '\0';

        // Appel de la fonction pour remplir la structure avec les valeurs de la ligne
        parseCSVLine(line, &drivers[i]);

        // Incrémenter l'indice du tableau
        i++;
    }

    // Fermeture du fichier
    fclose(file);

    // Affichage des résultats
    printf("nb: %d\n", drivers[num].nb);
    printf("num: %d\n", drivers[num].num);
    printf("prenom: %s\n", drivers[num].prenom);
    printf("nom: %s\n", drivers[num].nom);
    printf("team: %s\n", drivers[num].team);
    printf("naissance: %s\n", drivers[num].naissance);
    printf("nationalite: %s\n\n", drivers[num].nationalite);

    return 0;
}

#endif