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
int readCSVFile(const char *Namefile, struct Pilot pilots[], int *rowIndex) {
    FILE *file = fopen(Namefile, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du file");
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

/// CIRCUITS.CSV

#endif