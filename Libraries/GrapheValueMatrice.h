/* GrapheValueMatrice.h */
#ifndef GRAPHE_VALUE_MATRICE_H
#define GRAPHE_VALUE_MATRICE_H

/* Graph with weights on arcs.
   We assume positive integer weights.
   weight[i][j] = 0 means "no edge". */

typedef struct {
    int nb_sommets;
    int capacite;
    int oriente;
    int **weight;  /* 0 = no edge, >0 = weight */
} GrapheValueMatrice;

/* Creation / destruction */
GrapheValueMatrice *graphe_value_matrice_creer(int capacite_initiale, int oriente);
void graphe_value_matrice_detruire(GrapheValueMatrice *g);

/* Vertices */
int  graphe_value_matrice_ajouter_sommet(GrapheValueMatrice *g);
void graphe_value_matrice_supprimer_sommet(GrapheValueMatrice *g, int v);

/* Edges / arcs with weights */
void graphe_value_matrice_ajouter_arc(GrapheValueMatrice *g, int u, int v, int poids);
void graphe_value_matrice_supprimer_arc(GrapheValueMatrice *g, int u, int v);
int  graphe_value_matrice_existe_arc(const GrapheValueMatrice *g, int u, int v);

/* Weight operations */
int  graphe_value_matrice_obtenir_poids(const GrapheValueMatrice *g, int u, int v);
void graphe_value_matrice_modifier_poids(GrapheValueMatrice *g, int u, int v, int nouveau_poids);

/* Info */
int graphe_value_matrice_ordre(const GrapheValueMatrice *g);
int graphe_value_matrice_degre_sortant(const GrapheValueMatrice *g, int v);
int graphe_value_matrice_degre_entrant(const GrapheValueMatrice *g, int v);
int graphe_value_matrice_degre(const GrapheValueMatrice *g, int v);
int graphe_value_matrice_est_oriente(const GrapheValueMatrice *g);

#endif /* GRAPHE_VALUE_MATRICE_H */
