// customCSV.h
#ifndef CUSTOMCSV_H
#define CUSTOMCSV_H

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
    float sectorTimes[3];
    float bestLapTime;
    float totalTime;
    int pit;
    int out;
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
    int Num;
    char City[50];
    char DATE[20];
    char Country[50];
    char Name[100];
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
        sscanf(row, "%d[^;];%49[^;];%49[^;];%49[^;];%49[^;];%f;%49[^;];%49[^;];%9[^;]",
               &tracks[i].Num, tracks[i].City, tracks[i].DATE, tracks[i].Country,
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

// Fonction pour obtenir une piste par son index
struct Track getTrackByIndex(const char *tracksFilePath, int index) {
    FILE *file = fopen(tracksFilePath, "r");
    if (!file) {
        fprintf(stderr, "Erreur d'ouverture du fichier des pistes.\n");
        exit(EXIT_FAILURE);
    }

    // Ignorer la première ligne (en-têtes)
    char line[256];
    fgets(line, sizeof(line), file);

    // Parcourir les lignes pour trouver la piste correspondante
    int currentIndex = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (currentIndex == index) {
            struct Track track;
            sscanf(line, "%d,%49[^,],%19[^,],%49[^,],%99[^,],%f,%49[^,],%49[^,],%19[^,\n]",
                   &track.Num, track.City, track.DATE, track.Country, track.Name,
                   &track.Size, track.Race, track.TrackName, track.Date);

            fclose(file);
            return track;
        }

        currentIndex++;
    }

    fclose(file);

    // Si l'index est en dehors de la plage des pistes, retourner une piste avec un index invalide
    struct Track invalidTrack = {-1, "", "", "", "", 0.0, "", "", ""};
    return invalidTrack;
}

#endif