// customJSON.h
#ifndef CUSTOMJSON_H
#define CUSTOMJSON_H

#include <stdio.h>
#include <stdlib.h>
#include "../cJSON/cJSON.h"
#include "customCSV.h"

// Fonction pour lire le fichier JSON et retourner l'objet racine
cJSON *readJSONFile(const char *jsonFilePath) {
    FILE *file = fopen(jsonFilePath, "r");
    if (!file) {
        fprintf(stderr, "Erreur d'ouverture du fichier JSON.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *jsonBuffer = malloc(fileSize + 1);
    if (!jsonBuffer) {
        fclose(file);
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return NULL;
    }

    fread(jsonBuffer, 1, fileSize, file);
    fclose(file);

    jsonBuffer[fileSize] = '\0';  // Ajouter le caractère de fin de chaîne

    // Analyser le JSON
    cJSON *root = cJSON_Parse(jsonBuffer);

    free(jsonBuffer);

    if (!root) {
        fprintf(stderr, "Erreur lors de l'analyse du JSON.\n");
        return NULL;
    }

    return root;
}

// Fonction pour libérer la mémoire occupée par l'objet racine JSON
void freeJSONRoot(cJSON *root) {
    cJSON_Delete(root);
}

// Fonction pour récupérer la valeur de last_track à partir de l'objet racine JSON
int getLastTrackValue(cJSON *root) {
    // Récupérer la valeur de last_track
    cJSON *lastTrackJSON = cJSON_GetObjectItemCaseSensitive(root, "last_track");
    if (!cJSON_IsNumber(lastTrackJSON)) {
        fprintf(stderr, "Erreur : last_track n'est pas un nombre dans le JSON.\n");
        return -1;
    }

    return lastTrackJSON->valueint;
}

// Fonction pour mettre à jour la valeur de last_track dans le fichier JSON
void updateLastTrack(const char *jsonFilePath, const char *csvFilePath) {
    struct Track tracks[100];
    int numberOfCircuits = readTracksCSVFile(csvFilePath, tracks);

    // Lire le fichier JSON
    cJSON *root = readJSONFile(jsonFilePath);
    if (!root) {
        fprintf(stderr, "Erreur lors de la lecture du fichier JSON.\n");
        return;
    }

    // Récupérer la valeur actuelle de last_track
    int currentLastTrackValue = getLastTrackValue(root);

    // Incrémenter la valeur de last_track
    cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "last_track"), currentLastTrackValue + 1);

    // Si la nouvelle valeur est plus grande que le nombre de circuits, afficher "CHAMPIONNAT TERMINE"
    if (currentLastTrackValue + 1 > numberOfCircuits) {
        printf("CHAMPIONNAT TERMINE\n");
    }

    // Réécrire le fichier JSON mis à jour
    FILE *file = fopen(jsonFilePath, "w");
    if (!file) {
        fprintf(stderr, "Erreur d'ouverture du fichier JSON pour écriture.\n");
        freeJSONRoot(root);
        return;
    }

    fprintf(file, "%s", cJSON_Print(root));
    fclose(file);

    // Libérer la mémoire
    freeJSONRoot(root);
}

#endif