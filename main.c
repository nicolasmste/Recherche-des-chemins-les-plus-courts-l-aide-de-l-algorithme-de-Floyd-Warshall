#include <stdio.h>
#include <stdlib.h>
#define MAX_V 100

typedef struct edge {
    int existe; // 1 s'il y a une arête 0 sinon
    int poids; //poids de l'arête
} Edge;

typedef struct Graph {
    int nbvertexes; //nombre de sommets
    Edge matrice[MAX_V][MAX_V]; //matrice d'adjacence
} Graph;


/* initialiser unIgraphe à vide  */
void init_graphe(Graph *g) {
    g->nbvertexes = 0;
    for (int i = 0; i < MAX_V; i++) {
        for (int j = 0; j < MAX_V; j++) {
            g->matrice[i][j].existe = 0;
            g->matrice[i][j].poids = 0;
        }
    }
}

/* Lit un Graphe dans un fichier */
Graph charger_graphe(const char *nom_fichier) {
    Graph g;
    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    init_graphe(&g);
    if (fscanf(fichier, "%d", &g.nbvertexes) != 1) { //Lecture d'un caractère nb de sommets
        fprintf(stderr, "Erreur de format: nombre de sommets manquant.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    if (g.nbvertexes > MAX_V) {
        fprintf(stderr, "Erreur: Le graphe dépasse la capacité MAX_V (%d).\n", MAX_V);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    int src, dest, poids;
    while (fscanf(fichier, "%d %d %d", &src, &dest, &poids) != EOF) {
        if (src >= 0 && src < g.nbvertexes && dest >= 0 && dest < g.nbvertexes) {
            g.matrice[src][dest].existe = 1;
            g.matrice[src][dest].poids = poids; //JCP SI on prend en compte les graphe non orientés.

            // g.matrice[dest][src].existe = 1; g.matrice[dest][src].poids = poids;
        } else {
            fprintf(stderr, "Avertissement: sommet invalide ignoré (%d -> %d)\n", src, dest);
        }
    }
    fclose(fichier);
    return g;
}

void afficherGraphe(Graph g) {
    printf("Graphe avec %d sommets :\n", g.nbvertexes);
    for (int i = 0; i < g.nbvertexes; i++) {
        for (int j = 0; j < g.nbvertexes; j++) {
            if (g.matrice[i][j].existe) {
                printf("%3d ", g.matrice[i][j].poids);
            } else {
                printf(" - ");
            }
        }
        printf("\n");
    }
}

void afficherMatriceL(Edge L[][MAX_V], int n) {
    printf("\nMatrice des plus courts chemins (poids) :\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!L[i][j].existe)
                printf(" Inf ");
            else
                printf("%3d ", L[i][j].poids);
        }
        printf("\n");
    }
}

void afficherMatriceP(int P[][MAX_V], int n) {
    printf("\nMatrice des prédécesseurs P :\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", P[i][j]);
        }
        printf("\n");
    }
}

void floydWarshall(const Graph graphe,Edge L[][MAX_V], int P[][MAX_V]) {
    int n = graphe.nbvertexes;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            L[i][j]=graphe.matrice[i][j];
            if (i == j) {
                L[i][j].existe = 1;
                L[i][j].poids = 0;
                P[i][j] = i;
            }
            else if (graphe.matrice[i][j].existe) {
                P[i][j] = i;
            }
            else {
                P[i][j] = i;
            }
        }
    }  ///AMBIGUITE 2 ET 1 DANS LA MEME CASE
    for (int k =0;k<n;k++) {
        for (int i =0;i<n;i++){
            for (int j =0;j<n;j++){
                if (L[i][k].existe && L[k][j].existe) {

                    if (!L[i][j].existe ||
                        L[i][k].poids + L[k][j].poids < L[i][j].poids) {
                        L[i][j].existe = 1;
                        L[i][j].poids = L[i][k].poids + L[k][j].poids;
                        P[i][j] = P[k][j];
                        }
                }
            }
        }
    }
}


int main(void) {

    int run =1;
    while (run) {
        char nom_fichier[MAX_V];
        Edge L[MAX_V][MAX_V];
        int  P[MAX_V][MAX_V];
        printf("Bonjour fichier ?\n");
        scanf("%s", nom_fichier);
        Graph monGraphe = charger_graphe(nom_fichier);
        afficherGraphe(monGraphe);
        floydWarshall(monGraphe,L,P);
        afficherMatriceL(L,monGraphe.nbvertexes);
        afficherMatriceP(P,monGraphe.nbvertexes);

        run = 0;
    }
    return 0;
}

/*à faire :
 * Séparer le code en plusieurs fichiers + make file ou autre chose pour faire comme une librairie
 * Afficher Pour l'utilisateur les chemins
 * circuit absorband -- effectué code à rajouter
 * Afficher l'itération, P et L à chaque itération -- Prend 2 secondes
 *Améliorer l'interface utilisateur plus la boucle principame
 * Rendre les matrices plus lisible
 * Vérif représentation matricielle
 *
 * Question à poser :
 * "LES GRAPHES SERONT indiqués par des numéros" ???
 * Pas de graphe non-orienté ??
 * Doit on montrer le cas 1 OU 2
 */