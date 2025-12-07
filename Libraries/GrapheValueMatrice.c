/* GrapheValueMatrice.c */
#include <stdio.h>
#include <stdlib.h>
#include "GrapheValueMatrice.h"

static int **allouer_matrice(int n) {
    int **m = (int **)malloc(n * sizeof(int *));
    if (!m) return NULL;
    for (int i = 0; i < n; ++i) {
        m[i] = (int *)calloc(n, sizeof(int)); /* initialized to 0 = no edge */
        if (!m[i]) {
            for (int j = 0; j < i; ++j) free(m[j]);
            free(m);
            return NULL;
        }
    }
    return m;
}

static void liberer_matrice(int **m, int n) {
    if (!m) return;
    for (int i = 0; i < n; ++i) free(m[i]);
    free(m);
}

static int redimensionner(GrapheValueMatrice *g, int nouvelle_capacite) {
    int **nouvelle = allouer_matrice(nouvelle_capacite);
    if (!nouvelle) return 0;
    for (int i = 0; i < g->nb_sommets; ++i)
        for (int j = 0; j < g->nb_sommets; ++j)
            nouvelle[i][j] = g->weight[i][j];
    liberer_matrice(g->weight, g->capacite);
    g->weight = nouvelle;
    g->capacite = nouvelle_capacite;
    return 1;
}

/* PUBLIC API */

GrapheValueMatrice *graphe_value_matrice_creer(int capacite_initiale, int oriente) {
    GrapheValueMatrice *g = (GrapheValueMatrice *)malloc(sizeof(GrapheValueMatrice));
    if (!g) return NULL;
    if (capacite_initiale <= 0) capacite_initiale = 1;

    g->weight = allouer_matrice(capacite_initiale);
    if (!g->weight) {
        free(g);
        return NULL;
    }
    g->nb_sommets = 0;
    g->capacite   = capacite_initiale;
    g->oriente    = oriente ? 1 : 0;
    return g;
}

void graphe_value_matrice_detruire(GrapheValueMatrice *g) {
    if (!g) return;
    liberer_matrice(g->weight, g->capacite);
    free(g);
}

int graphe_value_matrice_ajouter_sommet(GrapheValueMatrice *g) {
    if (!g) return -1;
    if (g->nb_sommets == g->capacite) {
        int nouvelle_capacite = g->capacite * 2;
        if (!redimensionner(g, nouvelle_capacite)) return -1;
    }
    int v = g->nb_sommets;
    g->nb_sommets++;
    return v;
}

void graphe_value_matrice_supprimer_sommet(GrapheValueMatrice *g, int v) {
    if (!g) return;
    if (v < 0 || v >= g->nb_sommets) return;

    int last = g->nb_sommets - 1;
    if (v != last) {
        for (int j = 0; j < g->nb_sommets; ++j)
            g->weight[v][j] = g->weight[last][j];
        for (int i = 0; i < g->nb_sommets; ++i)
            g->weight[i][v] = g->weight[i][last];
    }
    for (int j = 0; j < g->nb_sommets; ++j) {
        g->weight[last][j] = 0;
        g->weight[j][last] = 0;
    }
    g->nb_sommets--;
}

void graphe_value_matrice_ajouter_arc(GrapheValueMatrice *g, int u, int v, int poids) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;
    if (poids <= 0) return; /* we assume strictly positive weights */
    g->weight[u][v] = poids;
    if (!g->oriente)
        g->weight[v][u] = poids;
}

void graphe_value_matrice_supprimer_arc(GrapheValueMatrice *g, int u, int v) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;
    g->weight[u][v] = 0;
    if (!g->oriente)
        g->weight[v][u] = 0;
}

int graphe_value_matrice_existe_arc(const GrapheValueMatrice *g, int u, int v) {
    if (!g) return 0;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return 0;
    return g->weight[u][v] > 0;
}

int graphe_value_matrice_obtenir_poids(const GrapheValueMatrice *g, int u, int v) {
    if (!g) return 0;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return 0;
    return g->weight[u][v]; /* 0 means no edge */
}

void graphe_value_matrice_modifier_poids(GrapheValueMatrice *g, int u, int v, int nouveau_poids) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;
    if (nouveau_poids <= 0) {
        /* treat as removing edge */
        graphe_value_matrice_supprimer_arc(g, u, v);
    } else {
        g->weight[u][v] = nouveau_poids;
        if (!g->oriente)
            g->weight[v][u] = nouveau_poids;
    }
}

int graphe_value_matrice_ordre(const GrapheValueMatrice *g) {
    if (!g) return 0;
    return g->nb_sommets;
}

int graphe_value_matrice_degre_sortant(const GrapheValueMatrice *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    for (int j = 0; j < g->nb_sommets; ++j)
        if (g->weight[v][j] > 0) deg++;
    return deg;
}

int graphe_value_matrice_degre_entrant(const GrapheValueMatrice *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    for (int i = 0; i < g->nb_sommets; ++i)
        if (g->weight[i][v] > 0) deg++;
    return deg;
}

int graphe_value_matrice_degre(const GrapheValueMatrice *g, int v) {
    return graphe_value_matrice_degre_sortant(g, v);
}

int graphe_value_matrice_est_oriente(const GrapheValueMatrice *g) {
    if (!g) return 0;
    return g->oriente;
}
