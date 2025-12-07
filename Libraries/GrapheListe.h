/* GrapheListe.h */
#ifndef GRAPHE_LISTE_H
#define GRAPHE_LISTE_H

/* Node in adjacency list */
typedef struct ArcListe {
    int dest;
    struct ArcListe *suivant;
} ArcListe;

typedef struct {
    int nb_sommets;
    int capacite;
    int oriente;    /* 0 = non oriented, 1 = oriented */
    ArcListe **adj; /* adj[i] = list of neighbors of i */
} GrapheListe;

/* Creation / destruction */
GrapheListe *graphe_liste_creer(int capacite_initiale, int oriente);
void graphe_liste_detruire(GrapheListe *g);

/* Vertices */
int  graphe_liste_ajouter_sommet(GrapheListe *g);
void graphe_liste_supprimer_sommet(GrapheListe *g, int v);

/* Edges / arcs */
void graphe_liste_ajouter_arc(GrapheListe *g, int u, int v);
void graphe_liste_supprimer_arc(GrapheListe *g, int u, int v);
int  graphe_liste_existe_arc(GrapheListe *g, int u, int v);

/* Info */
int graphe_liste_ordre(const GrapheListe *g);
int graphe_liste_degre_sortant(const GrapheListe *g, int v);
int graphe_liste_degre_entrant(const GrapheListe *g, int v);
int graphe_liste_degre(const GrapheListe *g, int v);
int graphe_liste_est_oriente(const GrapheListe *g);

/* Helper to iterate neighbors */
ArcListe *graphe_liste_voisins(const GrapheListe *g, int v);

#endif /* GRAPHE_LISTE_H */
