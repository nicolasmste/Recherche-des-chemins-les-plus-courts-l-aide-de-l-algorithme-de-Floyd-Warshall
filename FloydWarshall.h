#ifndef FLOYDWARSHALL_H
#define FLOYDWARSHALL_H
#define MAX_V 100 // nombre max d'edge d'un graphe
#define MAX_PRED 100 // nombre max de prédécesseurs pour une case
#define COL_WIDTH 5 //Pour s'assurer d'avoir un tableau lisible

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

void init_graphe(Graph *g); //Initialise un graphe vide graphe
Graph charger_graphe(const char *nom_fichier); //charge le graphe souhaité
void afficherGraphe(Graph g);//affiche e graphe chargé
void afficherMatriceL(Edge L[][MAX_V], int n);//Affiche la matrice
void afficherMatriceP(Predecesseurs P[][MAX_V], int n); //Afficher une matrice
void ajouterPredecesseur(Predecesseurs *p, int pred); //ajoute un predecesseur
void resetPredecesseurs(Predecesseurs *p);//Reset les predecesseurs
int floydWarshall(const Graph graphe, Edge L[][MAX_V], Predecesseurs P[][MAX_V]); //Effectue l'algorithme
void reconstruireTousLesCheminsIteratif(int depart, int arrivee, Predecesseurs P[][MAX_V], Edge L[][MAX_V]); //reconstruit tous les chemins de depart à arrivee de manière itérative

#endif