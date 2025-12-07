
/* GrapheMatrice.h */
#ifndef GRAPHE_MATRICE_H
#define GRAPHE_MATRICE_H

/* Graph represented by adjacency matrix (0/1).
   Vertices are numbered from 0 to nb_sommets-1. */

typedef struct {
    int nb_sommets;   /* current number of vertices (order) */
    int capacite;     /* capacity of adjacency matrix */
    int oriente;      /* 0 = non oriented, 1 = oriented */
    int **adj;        /* adj[i][j] = 1 if edge i->j exists, 0 otherwise */
} GrapheMatrice;

/* Creation / destruction */
GrapheMatrice *graphe_matrice_creer(int capacite_initiale, int oriente);
void graphe_matrice_detruire(GrapheMatrice *g);

/* Vertices */
int graphe_matrice_ajouter_sommet(GrapheMatrice *g);   /* returns index of new vertex */
void graphe_matrice_supprimer_sommet(GrapheMatrice *g, int v);

/* Edges / arcs */
void graphe_matrice_ajouter_arc(GrapheMatrice *g, int u, int v);
void graphe_matrice_supprimer_arc(GrapheMatrice *g, int u, int v);
int graphe_matrice_existe_arc(const GrapheMatrice *g, int u, int v);

/* Info */
int graphe_matrice_ordre(const GrapheMatrice *g);
int graphe_matrice_degre_sortant(const GrapheMatrice *g, int v);
int graphe_matrice_degre_entrant(const GrapheMatrice *g, int v);
int graphe_matrice_degre(const GrapheMatrice *g, int v); /* for non-oriented */
int graphe_matrice_est_oriente(const GrapheMatrice *g);

#endif /* GRAPHE_MATRICE_H */
