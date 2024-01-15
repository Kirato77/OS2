#include <stdio.h>
#include "libs/utils/scores.h"

// Function to simulate a normal weekend
void simulateNormalWeekend() {
    printf("Simulation du week-end en mode normal...\n");
}

// Function to simulate a special weekend
void simulateSpecialWeekend() {
    printf("Simulation du week-end en mode spécial...\n");
}

int main() {
    int choice;
    char response;

    /// Print scores
    ScoreTable *scoreTable = getScoreTable("./data/data.json", "./data/pilots.csv");

    // Trier le tableau des scores
    sortScoreTable(scoreTable);

    // Afficher le tableau des scores
    printf("Dernier circuit : %d\n", scoreTable->lastTrack);
    printf("Tableau des scores :\n");
    for (int i = 0; i < scoreTable->numScores; ++i) {
        printf("Pilote %d : %d points\n", scoreTable->scores[i].pilotNumber, scoreTable->scores[i].points);
    }

    // Libérer la mémoire allouée
    freeScoreTable(scoreTable);

    /// Menu
    while (1) {
        // Ask the user if they want to start the next weekend
        printf("Voulez-vous lancer le prochain week-end ? (O/N): ");
        scanf(" %c", &response);

        if (response == 'O' || response == 'o') {
            // Ask the user to choose the mode
            printf("Choisissez le mode :\n");
            printf("1. Mode Normal\n");
            printf("2. Mode Special\n");
            printf("3. Exit\n");
            printf("Votre choix : ");

            // Lire le choix et vérifier s'il est valide
            while (scanf("%d", &choice) != 1 || (choice < 1 || choice > 3)) {
                printf("Choix invalide. Veuillez choisir à nouveau : ");

                // Vider le tampon d'entrée
                while (getchar() != '\n');
            }

            switch (choice) {
                case 1:
                    simulateNormalWeekend();
                    break;
                case 2:
                    simulateSpecialWeekend();
                    break;
                case 3:
                    // Revenir à l'étape du début
                    continue;
                default:
                    printf("Choix invalide. Veuillez choisir à nouveau.\n");
            }
        } else if (response == 'N' || response == 'n') {
            // Sortir de la boucle infinie si la réponse est 'N' ou 'n'
            break;
        } else {
            printf("Réponse invalide. Veuillez répondre avec O/N.\n");
            // Vider le tampon d'entrée
            while (getchar() != '\n');
        }
    }

    printf("Programme terminé.\n");

    return 0;
}
