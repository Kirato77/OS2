// scores.h
#ifndef SCORES_H
#define SCORES_H

#include <stdio.h>
#include <stdlib.h>
#include "customCSV.h"
#include "../cjson/cJSON.h"

// Structure pour représenter un score d'un pilote
typedef struct {
    int pilotNumber;
    int points;
} PilotScore;

// Structure pour représenter le tableau des scores
typedef struct {
    int lastTrack;
    PilotScore *scores;
    int numScores;
} ScoreTable;

// Fonction pour libérer la mémoire allouée pour le tableau des scores
void freeScoreTable(ScoreTable *table) {
    free(table->scores);
    free(table);
}

// Comparateur pour la fonction qsort
int compareScores(const void *a, const void *b) {
    const PilotScore *scoreA = (const PilotScore *) a;
    const PilotScore *scoreB = (const PilotScore *) b;

    // Tri en ordre décroissant (du plus grand au plus petit)
    return scoreB->points - scoreA->points;
}

// Fonction pour trier le tableau des scores
void sortScoreTable(ScoreTable *table) {
    qsort(table->scores, table->numScores, sizeof(PilotScore), compareScores);
}

// Function to read the CSV file and initialize scores if data.json is empty
void initializeScoresFromCSV(ScoreTable *scoreTable, const char *filename) {
    struct Pilot pilots[100];
    int rowIndex = 0;

    if (readPilotsCSVFile(filename, pilots, &rowIndex) != 0) {
        fprintf(stderr, "Erreur lors de la lecture du fichier CSV.\n");
        exit(EXIT_FAILURE);
    }

    // Check if the scores array is empty in the JSON file
    if (scoreTable->numScores == 0) {
        // Initialize scores from the CSV data
        scoreTable->numScores = rowIndex;
        scoreTable->scores = (PilotScore *)malloc(sizeof(PilotScore) * scoreTable->numScores);

        for (int i = 0; i < rowIndex; ++i) {
            scoreTable->scores[i].pilotNumber = pilots[i].Num;
            scoreTable->scores[i].points = 0;
        }
    }
}

void writeScoresToJSON(ScoreTable *scoreTable, const char *filename) {
    cJSON *jsonRoot = cJSON_CreateObject();
    cJSON_AddNumberToObject(jsonRoot, "last_circuit", scoreTable->lastTrack);

    cJSON *jsonScores = cJSON_CreateArray();
    for (int i = 0; i < scoreTable->numScores; ++i) {
        cJSON *jsonScore = cJSON_CreateObject();
        cJSON_AddNumberToObject(jsonScore, "pilot", scoreTable->scores[i].pilotNumber);
        cJSON_AddNumberToObject(jsonScore, "points", scoreTable->scores[i].points);
        cJSON_AddItemToArray(jsonScores, jsonScore);
    }

    cJSON_AddItemToObject(jsonRoot, "scores", jsonScores);

    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du file data.json pour l'écriture.\n");
        cJSON_Delete(jsonRoot); // Free cJSON root before exit
        exit(EXIT_FAILURE);
    }

    char *jsonData = cJSON_Print(jsonRoot);
    fputs(jsonData, file);
    fclose(file);

    cJSON_Delete(jsonRoot);
    free(jsonData);
}

// Function to check if the scores array is empty in the JSON file
int isScoresEmpty(cJSON *jsonScores) {
    return cJSON_IsArray(jsonScores) && cJSON_GetArraySize(jsonScores) == 0;
}

// Fonction pour construire le tableau des scores à partir d'un file JSON
ScoreTable *getScoreTable(const char *jsonfile, const char *csvfile) {
    FILE *file = fopen(jsonfile, "r");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", jsonfile);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *jsonData = (char *)malloc(fileSize + 1);
    fread(jsonData, 1, fileSize, file);
    fclose(file);

    jsonData[fileSize] = '\0';

    cJSON *jsonRoot = cJSON_Parse(jsonData);
    free(jsonData);

    if (!jsonRoot) {
        fprintf(stderr, "Erreur lors de l'analyse du fichier JSON.\n");
        exit(EXIT_FAILURE);
    }

    cJSON *jsonScores = cJSON_GetObjectItemCaseSensitive(jsonRoot, "scores");

    ScoreTable *scoreTable = (ScoreTable *)malloc(sizeof(ScoreTable));
    scoreTable->lastTrack = cJSON_GetObjectItemCaseSensitive(jsonRoot, "last_track")->valueint;

    if (isScoresEmpty(jsonScores)) {
        // If scores array is empty, initialize from CSV
        initializeScoresFromCSV(scoreTable, csvfile);

        // Save the initialized scores to data.json
        writeScoresToJSON(scoreTable, jsonfile);

        // Read JSON again after initialization
        cJSON_Delete(jsonRoot);
        scoreTable = getScoreTable(jsonfile, csvfile);
    } else {
        // Otherwise, read scores from JSON
        scoreTable->numScores = cJSON_GetArraySize(jsonScores);
        scoreTable->scores = (PilotScore *)malloc(sizeof(PilotScore) * scoreTable->numScores);

        for (int i = 0; i < scoreTable->numScores; ++i) {
            cJSON *jsonScore = cJSON_GetArrayItem(jsonScores, i);
            scoreTable->scores[i].pilotNumber = cJSON_GetObjectItemCaseSensitive(jsonScore, "pilot")->valueint;
            scoreTable->scores[i].points = cJSON_GetObjectItemCaseSensitive(jsonScore, "points")->valueint;
        }
        cJSON_Delete(jsonRoot);
    }

    return scoreTable;
}

#endif