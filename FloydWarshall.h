#ifndef FLOYDWARSHALL_H
#define FLOYDWARSHALL_H
#define MAX_V 100 // nombre max d'edge d'un graphe
#define MAX_PRED 100 // nombre max de prédécesseurs pour une case
#define COL_WIDTH 5

typedef struct edge { //representation d'une arete
    int existe; // 1 s'il y a une arête 0 sinon
    int poids; //poids de l'arête
} Edge;

typedef struct Graph { //représentation d'un graphe
    int nbvertexes; //nombre de sommets
    Edge matrice[MAX_V][MAX_V]; //matrice d'adjacence
} Graph;

typedef struct {  //representation des predecesseurs
    int predecesseurs[MAX_PRED]; // liste des predecesseurs
    int nb_pred; // nombre de prédécesseurs actuels
} Predecesseurs;

void init_graphe(Graph *g);
Graph charger_graphe(const char *nom_fichier);
void afficherGraphe(Graph g);
void afficherMatriceL(Edge L[][MAX_V], int n);
void afficherMatriceP(Predecesseurs P[][MAX_V], int n);
void ajouterPredecesseur(Predecesseurs *p, int pred);
void resetPredecesseurs(Predecesseurs *p);
int floydWarshall(const Graph graphe, Edge L[][MAX_V], Predecesseurs P[][MAX_V]);
void reconstruireTousLesCheminsIteratif(int depart, int arrivee, Predecesseurs P[][MAX_V], Edge L[][MAX_V]);

#endif