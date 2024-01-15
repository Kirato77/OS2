// customCSV.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// PILOTS.CSV
struct Pilot {
    int nb;
    int Num;
    char Firstname[50];
    char Name[50];
    char Team[50];
    char Birthday[15];
    char Nationality[30];
};

// Fonction pour lire un file CSV et renvoyer un tableau de structures
int readPilotsCSVFile(const char *filename, struct Pilot pilots[], int *rowIndex) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    char row[1024];

    if (fgets(row, sizeof(row), file) == NULL) {
        fclose(file);
        return 1; // Gestion de l'erreur si le file est vide
    }

    while (fgets(row, sizeof(row), file)) {
        row[strcspn(row, "\n")] = 0;

        if (row[0] == '\0') {
            continue;
        }

        char *token = strtok(row, ",");
        int fieldIndex = 0;
        while (token != NULL) {
            switch (fieldIndex) {
                case 0:
                    pilots[*rowIndex].nb = atoi(token);
                    break;
                case 1:
                    pilots[*rowIndex].Num = atoi(token);
                    break;
                case 2:
                    strcpy(pilots[*rowIndex].Firstname, token);
                    break;
                case 3:
                    strcpy(pilots[*rowIndex].Name, token);
                    break;
                case 4:
                    strcpy(pilots[*rowIndex].Team, token);
                    break;
                case 5:
                    strcpy(pilots[*rowIndex].Birthday, token);
                    break;
                case 6:
                    strcpy(pilots[*rowIndex].Nationality, token);
                    break;
                default:
                    break;
            }
            fieldIndex++;
            token = strtok(NULL, ",");
        }
        (*rowIndex)++;
    }

    fclose(file);
    return 0;
}

/// TRACKS.CSV
#define MAX_ROWS 25

struct Track {
    char Num[50];
    char City[50];
    char DATE[10];
    char Country[50];
    char Name[50];
    float Size;
    char Race[50];
    char TrackName[50];
    char Date[10];
};

int readTracksCSVFile(const char *filename, struct Track tracks[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    char row[1024];
    int i = 0;

    // Ignorer la première ligne (headers)
    fgets(row, sizeof(row), file);

    while (fgets(row, sizeof(row), file) != NULL) {
        // Utiliser sscanf pour extraire les valeurs de la ligne
        sscanf(row, "%49[^;];%49[^;];%9[^;];%49[^;];%49[^;];%f;%49[^;];%49[^;];%9[^;]",
               tracks[i].Num, tracks[i].City, tracks[i].DATE, tracks[i].Country,
               tracks[i].Name, &tracks[i].Size, tracks[i].Race, tracks[i].TrackName,
               tracks[i].Date);

        i++;

        if (i >= MAX_ROWS) {
            break;  // Limiter le nombre de lignes lues pour éviter le dépassement de tableau
        }
    }

    fclose(file);

    return i;  // Retourne le le nombre de lignes lues
}

#endif