#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FloydWarshall.h"

void init_graphe(Graph *g) {//Afin de creer un graphe
    g->nbvertexes = 0; //0 sommet
    for (int i = 0; i < MAX_V; i++) { //On ajoute le poids des aretes
        for (int j = 0; j < MAX_V; j++) {
            g->matrice[i][j].existe = 0;
            g->matrice[i][j].poids = 0;
        }
    }
}






Graph charger_graphe(const char *nom_fichier) { // On essaye de lire le fichier du graphe souhaité
    Graph g;

    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {  //Verification, securite
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    init_graphe(&g); // Si on arrive à le lire on initialise le graphe associe

    if (fscanf(fichier, "%d", &g.nbvertexes) != 1) { // On recupere le nombre de sommets
        fprintf(stderr, "Erreur de format: nombre de sommets manquant.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    if (g.nbvertexes > MAX_V) {  //Si le graphe possede trop de sommets : ERREUR
        printf( "Erreur: Le graphe dépasse la capacité maximale autorisée : %d.\n", MAX_V);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    int src, dest, poids;
    while (fscanf(fichier, "%d %d %d", &src, &dest, &poids) != EOF) { //On regarde jusqu a la fin du fichier les edges
        if (src >= 0 && src < g.nbvertexes && dest >= 0 && dest < g.nbvertexes) {
            g.matrice[src][dest].existe = 1;
            g.matrice[src][dest].poids = poids;
        } else {
            fprintf(stderr, "Attention !!!! Il y a un sommet invalide ignoré (%d -> %d)\n", src, dest);
        }
    }
    fclose(fichier); //On ferme le fichier
    return g;
}






void afficherGraphe(Graph g) {
    printf("Matrice d'adjacence (%d sommets) :\n\n", g.nbvertexes);

    printf("      ");
    for (int j = 0; j < g.nbvertexes; j++) {
        printf("%4d ", j);
    }
    printf("\n");

    printf("      ");
    for (int j = 0; j < g.nbvertexes; j++) {
        printf("-----");
    }
    printf("\n");

    for (int i = 0; i < g.nbvertexes; i++) {
        printf(" %3d |", i);

        for (int j = 0; j < g.nbvertexes; j++) {
            if (g.matrice[i][j].existe) {
                printf("%4d ", g.matrice[i][j].poids);
            } else {
                printf("  -  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}





void afficherMatriceL(Edge L[][MAX_V], int n) {
    printf("\nMatrice des plus courts chemins (poids) :\n\n");

    printf("      ");
    for (int j = 0; j < n; j++) {
        printf("%5d ", j);
    }
    printf("\n");

    printf("      ");
    for (int j = 0; j < n; j++) {
        printf("------");
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf(" %3d |", i);

        for (int j = 0; j < n; j++) {
            if (!L[i][j].existe) {
                printf("  Inf ");
            } else {
                printf("%5d ", L[i][j].poids);
            }
        }
        printf("\n");
    }
    printf("\n");
}






void afficherMatriceP(Predecesseurs P[][MAX_V], int n) {
    printf("\nMatrice des prédécesseurs P :\n\n");

    // 1. Affichage des en-têtes de colonnes
    printf("     "); // Espace pour l'index de ligne
    for (int j = 0; j < n; j++) {
        // %*d permet d'utiliser une largeur variable définie par COL_WIDTH
        printf("%*d ", COL_WIDTH, j);
    }
    printf("\n");

    // 2. Affichage de la ligne de séparation
    printf("     ");
    for (int j = 0; j < n; j++) {
        for (int k = 0; k <= COL_WIDTH; k++) printf("-"); // Tirets ajustés à la largeur
    }
    printf("\n");

    // 3. Affichage du contenu
    char buffer[50]; // Tampon pour construire la chaîne "{1,2}"

    for (int i = 0; i < n; i++) {
        // Affichage de l'index de ligne
        printf(" %3d |", i);

        for (int j = 0; j < n; j++) {
            // A. Construction de la chaîne pour cette cellule
            if (P[i][j].nb_pred == 0) {
                // Cas vide ou diagonale selon votre logique
                sprintf(buffer, "%d",i);
            } else {
                // Cas avec prédécesseurs : on ouvre l'accolade
                sprintf(buffer, "");
                char temp[10];

                for (int k = 0; k < P[i][j].nb_pred; k++) {
                    // On ajoute le nombre
                    sprintf(temp, "%d", P[i][j].predecesseurs[k]);
                    strcat(buffer, temp);

                    // On ajoute une virgule si ce n'est pas le dernier
                    if (k < P[i][j].nb_pred - 1) {
                        strcat(buffer, ",");
                    }
                }
                // On ferme l'accolade
                strcat(buffer, "");
            }

            // B. Affichage de la chaîne formatée
            // %*s aligne à droite. Utilisez %-*s pour aligner à gauche.
            printf("%*s ", COL_WIDTH, buffer);
        }
        printf("\n");
    }
    printf("\n");
}






// Fonction pour ajouter un prédécesseur s'il n'existe pas déjà
void ajouterPredecesseur(Predecesseurs *p, int pred) {
    // Vérifier si le prédécesseur existe déjà
    for (int i = 0; i < p->nb_pred; i++) {
        if (p->predecesseurs[i] == pred) {
            return; // Déjà présent
        }
    }

    // Ajouter le prédécesseur si on a de la place
    if (p->nb_pred < MAX_PRED) {
        p->predecesseurs[p->nb_pred] = pred;
        p->nb_pred++;
    } else {
        fprintf(stderr, "Attention: nombre maximum de prédécesseurs atteint!\n");
    }
}







// Réinitialiser la liste de prédécesseurs
void resetPredecesseurs(Predecesseurs *p) {
    p->nb_pred = 0;
}







int floydWarshall(const Graph graphe, Edge L[][MAX_V], Predecesseurs P[][MAX_V]) {
    int circ = 0;
    int n = graphe.nbvertexes;

    // Initialisation
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = graphe.matrice[i][j];
            P[i][j].nb_pred = 0;


            if (graphe.matrice[i][j].existe) {
                ajouterPredecesseur(&P[i][j], i);
            }
            else if (i == j) {
                L[i][j].existe = 1;
                L[i][j].poids = 0;
            }
        }
    }

    printf("\n=== INITIALISATION ===\n");
    afficherMatriceL(L, graphe.nbvertexes);
    afficherMatriceP(P, graphe.nbvertexes);



// Algorithme de Floyd-Warshall
    for (int k = 0; k < n; k++) {
        printf("\n=== ITERATION k=%d ===\n", k);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (L[i][k].existe && L[k][j].existe) {
                    int nouveau_poids = L[i][k].poids + L[k][j].poids;

                    if (!L[i][j].existe) {
                        // Nouveau chemin trouvé
                        L[i][j].existe = 1;
                        L[i][j].poids = nouveau_poids;
                        resetPredecesseurs(&P[i][j]);
                        // Copier tous les prédécesseurs de k vers j
                        for (int p = 0; p < P[k][j].nb_pred; p++) {
                            // Ne pas ajouter j comme prédécesseur de j
                            if (P[k][j].predecesseurs[p] != j) {
                                ajouterPredecesseur(&P[i][j], P[k][j].predecesseurs[p]);
                            }
                        }
                    }
                    else if (nouveau_poids < L[i][j].poids) {
                        // Chemin plus court trouvé
                        L[i][j].poids = nouveau_poids;
                        resetPredecesseurs(&P[i][j]);
                        for (int p = 0; p < P[k][j].nb_pred; p++) {
                            // Ne pas ajouter j comme prédécesseur de j
                            if (P[k][j].predecesseurs[p] != j) {
                                ajouterPredecesseur(&P[i][j], P[k][j].predecesseurs[p]);
                            }
                        }
                    }
                    else if (nouveau_poids == L[i][j].poids) {
                        // Chemin de même poids : ajouter les prédécesseurs alternatifs
                        for (int p = 0; p < P[k][j].nb_pred; p++) {
                            // Ne pas ajouter j comme prédécesseur de j
                            if (P[k][j].predecesseurs[p] != j) {
                                ajouterPredecesseur(&P[i][j], P[k][j].predecesseurs[p]);
                            }
                        }
                    }
                }
            }
        }

        afficherMatriceL(L, graphe.nbvertexes);
        afficherMatriceP(P, graphe.nbvertexes);
    }

    // Vérification des cycles négatifs
    for (int i = 0; i < n; i++) {
        if (L[i][i].existe && L[i][i].poids < 0) {
            printf("Il y a au moins un circuit absorbant dans ce graphe) !\n");
            return 1;
        }
    }

    return circ;
}









void reconstruireTousLesCheminsIteratif(int depart, int arrivee, Predecesseurs P[][MAX_V], Edge L[][MAX_V]) {
    printf("\n=== Reconstitution de TOUS les chemins de %d à %d ===\n", depart, arrivee);

    // 1. Vérifications de base
    if (!L[depart][arrivee].existe) {
        printf("Aucun chemin.\n");
        return;
    }
    if (depart == arrivee) {
        printf("Chemin : %d (Distance : 0)\n", depart);
        return;
    }


    // cheminStack : stocke les sommets du chemin en cours de construction (de l'arrivée vers le départ)
    int cheminStack[MAX_V];
    // indexStack : stocke à quel "numéro de prédécesseur" on est rendu pour chaque étape
    int indexStack[MAX_V];
    int depth = 0; // Hauteur actuelle de la pile

    // Initialisation : On commence par l'arrivée
    cheminStack[0] = arrivee;
    indexStack[0] = 0; // On commencera par explorer le 1er prédécesseur de l'arrivée

    int compteurChemins = 0;

    // 3. Boucle principale (Tant que la pile n'est pas vide)
    while (depth >= 0) {
        int courant = cheminStack[depth];

        // --- CAS A : On a atteint le point de départ ---
        if (courant == depart) {
            compteurChemins++;
            printf("Chemin %d : ", compteurChemins);

            // On affiche la pile. Attention, la pile est construite [Arrivée, ..., Départ]
            // Donc on l'affiche de la fin (depth) vers le début (0)
            for (int i = depth; i >= 0; i--) {
                printf("%d", cheminStack[i]);
                if (i > 0) printf(" -> ");
            }
            printf(" (Cout: %d)\n", L[depart][arrivee].poids);

            // Backtracking forcé pour chercher d'autres chemins
            depth--;
            continue;
        }

        // --- CAS B : Exploration des prédécesseurs ---

        // On récupère la liste des prédécesseurs pour aller de 'depart' à 'courant'
        Predecesseurs *predsInfo = &P[depart][courant];
        int idx = indexStack[depth]; // Où en étions-nous dans la liste des prédécesseurs ?

        if (idx < predsInfo->nb_pred) {
            // Il reste des prédécesseurs à explorer
            int pred = predsInfo->predecesseurs[idx];

            // 1. On prépare l'index pour la prochaine fois qu'on reviendra à ce niveau
            // (on passera au prédécesseur suivant)
            indexStack[depth]++;

            // 2. On empile le nouveau sommet (on avance)
            depth++;
            if (depth >= MAX_V) {
                printf("Erreur: Chemin trop long (boucle probable).\n");
                return;
            }
            cheminStack[depth] = pred;
            indexStack[depth] = 0; // Pour ce nouveau sommet, on commencera à l'index 0

        } else {
            // --- CAS C : Plus de prédécesseurs pour ce sommet (Cul-de-sac ou tout exploré) ---
            // On dépile (Backtracking)
            depth--;
        }
    }
}