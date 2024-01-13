#include <stdio.h>
#include <stdlib.h>

// Function to simulate a normal weekend
void simulateNormalWeekend() {
    printf("Simulation du week-end en mode normal...\n");
    // Add your simulation logic here
}

// Function to simulate a special weekend
void simulateSpecialWeekend() {
    printf("Simulation du week-end en mode spécial...\n");
    // Add your special simulation logic here
}

int main() {
    int choice;
    char response;

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
            scanf("%d", &choice);

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
        } else if (response != 'N' && response != 'n') {
            printf("Réponse invalide. Veuillez répondre avec O/N.\n");
        } else {
            // Sortir de la boucle infinie si la réponse est 'N' ou 'n'
            break;
        }
    }

    printf("Programme terminé.\n");

    return 0;
}
