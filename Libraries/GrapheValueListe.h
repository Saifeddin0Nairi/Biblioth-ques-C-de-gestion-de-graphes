/* GrapheValueListe.h */
#ifndef GRAPHE_VALUE_LISTE_H
#define GRAPHE_VALUE_LISTE_H

typedef struct ArcValueListe {
    int dest;
    int poids;
    struct ArcValueListe *suivant;
} ArcValueListe;

typedef struct {
    int nb_sommets;
    int capacite;
    int oriente;
    ArcValueListe **adj; /* list of neighbors with weights */
} GrapheValueListe;

/* Creation / destruction */
GrapheValueListe *graphe_value_liste_creer(int capacite_initiale, int oriente);
void graphe_value_liste_detruire(GrapheValueListe *g);

/* Vertices */
int  graphe_value_liste_ajouter_sommet(GrapheValueListe *g);
void graphe_value_liste_supprimer_sommet(GrapheValueListe *g, int v);

/* Edges / arcs */
void graphe_value_liste_ajouter_arc(GrapheValueListe *g, int u, int v, int poids);
void graphe_value_liste_supprimer_arc(GrapheValueListe *g, int u, int v);
int  graphe_value_liste_existe_arc(GrapheValueListe *g, int u, int v);

/* Weight operations */
int  graphe_value_liste_obtenir_poids(GrapheValueListe *g, int u, int v, int *poids);
void graphe_value_liste_modifier_poids(GrapheValueListe *g, int u, int v, int nouveau_poids);

/* Info */
int graphe_value_liste_ordre(const GrapheValueListe *g);
int graphe_value_liste_degre_sortant(const GrapheValueListe *g, int v);
int graphe_value_liste_degre_entrant(const GrapheValueListe *g, int v);
int graphe_value_liste_degre(const GrapheValueListe *g, int v);
int graphe_value_liste_est_oriente(const GrapheValueListe *g);

/* Helper to iterate neighbors */
ArcValueListe *graphe_value_liste_voisins(const GrapheValueListe *g, int v);

#endif /* GRAPHE_VALUE_LISTE_H */
