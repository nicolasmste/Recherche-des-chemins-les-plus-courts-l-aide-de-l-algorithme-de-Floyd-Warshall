#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FloydWarshall.h"
#include <string.h>

// Fonction utilitaire pour vider le buffer d'entrée (évite les bugs de scanf)
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

char* selectiongraphe(int entier){//On ouvre le graphe voulu
    switch(entier){
        case 1:
            return "graphe1.txt";
        case 2:
            return "graphe2.txt";
        case 3:
            return "graphe3.txt";
        case 4:
            return "graphe4.txt";
        case 5:
            return "graphe5.txt";
        case 6:
            return "graphe6.txt";
        case 7:
            return "graphe7.txt";
        case 8:
            return "graphe8.txt";
        case 9:
            return "graphe9.txt";
        case 10:
            return "graphe10.txt";
        case 11:
            return "graphe11.txt";
        case 12:
            return "graphe12.txt";
        case 13:
            return "graphe13.txt";
        default:
            printf("Veuillez rentrer un numéro entre 1 et 13;");
            return "faux";
    }
}

int main() {
    int choixMenu = 0;
    int entier;
    char nomFichier[30];

    printf("*******\n");
    printf("Projet Floyd-Warshall\n");
    printf("********\n\n");



    //  Boucle principale : Exécuter sur une suite de graphes sans relancer le programme
    do {
        // --- ÉTAPE 1 : Chargement du graphe ---
        // L'utilisateur indique le graphe à analyser
        printf("\n--- Chargement d'un nouveau graphe ---\n");
        printf("Entrez le nom du fichier du graphe  : ");
        scanf("%d",&entier);
        strcpy(nomFichier, selectiongraphe(entier));
        //  Chargement en mémoire
        if (strcmp(nomFichier,"faux") != 0) {
            Graph graphe = charger_graphe(nomFichier);


            //  À partir d'ici, on n'accède plus au fichier, tout est en mémoire.

            // --- ÉTAPE 2 : Affichage Initial ---
            //  Affichage sous forme matricielle

            printf("=== GRAPHE CHARGE ===\n");
            afficherGraphe(graphe);



            // --- ÉTAPE 3 : Algorithme de Floyd-Warshall ---
            // xécution et affichage des matrices intermédiaires
            printf("\n[Execution de Floyd-Warshall]\n");
            static Edge L[MAX_V][MAX_V]; //Static pour eviter les problemes de memoire
            static Predecesseurs P[MAX_V][MAX_V];
            int circuit = floydWarshall(graphe, L, P);

            // --- ÉTAPE 4 : Circuits Absorbants ---
            // Indication de la présence d'un circuit absorbant
            if (!circuit) {
                printf("\n=== RESULTAT FINAL ===\n");
                printf("Aucun circuit absorbant détecté.\n");

                printf("\n--- Recherche de chemins optimaux ---\n");
                int sommetDepart, sommetArrivee;
                int choixChemin = 1;

                while (1) {
                    //  Chemin ?
                    printf("\nSouhaitez-vous afficher un chemin ? (1: Oui, 0: Non) : ");
                    if (scanf("%d", &choixChemin) != 1) {
                        viderBuffer(); // Gérer les entrées non numériques
                        choixChemin = 0;
                    }

                    //  Si non, arrêter cette boucle
                    if (choixChemin == 0) {
                        break;
                    }

                    // Sommet de départ ? Sommet d'arrivée ?
                    printf("Sommet de depart ? : ");
                    scanf("%d", &sommetDepart);

                    printf("Sommet d'arrivee ? : ");
                    scanf("%d", &sommetArrivee);

                    // Validation basique des sommets
                    if (sommetDepart < 0 || sommetDepart >= graphe.nbvertexes ||
                        sommetArrivee < 0 || sommetArrivee >= graphe.nbvertexes) {
                        printf("Erreur : Les sommets doivent etre entre 0 et %d.\n", graphe.nbvertexes - 1);
                        } else {
                            // Affichage du chemin
                            reconstruireTousLesCheminsIteratif(sommetDepart, sommetArrivee, P,L);
                        }

                    // Recommencer ? (C'est implicite avec le while(1) et la question au début)
                }

            }else printf("L'affichage des chemins n'est pas possible.\n");

        }
        viderBuffer();
        // Demander si on veut traiter un autre graphe
        printf("\n---------------------------------------------------\n");
        printf("Voulez-vous traiter un autre graphe ? (1: Oui, 0: Non) : ");
        scanf("%d", &choixMenu);

    } while (choixMenu != 0);

    printf("Fin du programme.\n");
    return 0;
}