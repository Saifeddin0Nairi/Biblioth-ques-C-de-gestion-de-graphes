
/* GrapheMatrice.c */
#include <stdio.h>
#include <stdlib.h>
#include "GrapheMatrice.h"

static int **allouer_matrice(int n) {
    int **m = (int **)malloc(n * sizeof(int *));
    if (!m) return NULL;
    for (int i = 0; i < n; ++i) {
        m[i] = (int *)calloc(n, sizeof(int));
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

static int redimensionner(GrapheMatrice *g, int nouvelle_capacite) {
    int **nouvelle = allouer_matrice(nouvelle_capacite);
    if (!nouvelle) return 0;
    for (int i = 0; i < g->nb_sommets; ++i)
        for (int j = 0; j < g->nb_sommets; ++j)
            nouvelle[i][j] = g->adj[i][j];
    liberer_matrice(g->adj, g->capacite);
    g->adj = nouvelle;
    g->capacite = nouvelle_capacite;
    return 1;
}

/* PUBLIC API */

GrapheMatrice *graphe_matrice_creer(int capacite_initiale, int oriente) {
    GrapheMatrice *g = (GrapheMatrice *)malloc(sizeof(GrapheMatrice));
    if (!g) return NULL;
    if (capacite_initiale <= 0) capacite_initiale = 1;

    g->adj = allouer_matrice(capacite_initiale);
    if (!g->adj) {
        free(g);
        return NULL;
    }
    g->nb_sommets = 0;
    g->capacite   = capacite_initiale;
    g->oriente    = oriente ? 1 : 0;
    return g;
}

void graphe_matrice_detruire(GrapheMatrice *g) {
    if (!g) return;
    liberer_matrice(g->adj, g->capacite);
    free(g);
}

int graphe_matrice_ajouter_sommet(GrapheMatrice *g) {
    if (!g) return -1;
    if (g->nb_sommets == g->capacite) {
        int nouvelle_capacite = g->capacite * 2;
        if (!redimensionner(g, nouvelle_capacite)) return -1;
    }
    int v = g->nb_sommets;
    g->nb_sommets++;
    /* New row/column already initialized to 0 by calloc/redimensionner */
    return v;
}

void graphe_matrice_supprimer_sommet(GrapheMatrice *g, int v) {
    if (!g) return;
    if (v < 0 || v >= g->nb_sommets) return;

    int last = g->nb_sommets - 1;
    if (v != last) {
        /* Move last row into row v */
        for (int j = 0; j < g->nb_sommets; ++j)
            g->adj[v][j] = g->adj[last][j];
        /* Move last column into column v */
        for (int i = 0; i < g->nb_sommets; ++i)
            g->adj[i][v] = g->adj[i][last];
    }

    /* Clear last row/col (optional) */
    for (int j = 0; j < g->nb_sommets; ++j) {
        g->adj[last][j] = 0;
        g->adj[j][last] = 0;
    }

    g->nb_sommets--;
}

void graphe_matrice_ajouter_arc(GrapheMatrice *g, int u, int v) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;
    g->adj[u][v] = 1;
    if (!g->oriente)
        g->adj[v][u] = 1;
}

void graphe_matrice_supprimer_arc(GrapheMatrice *g, int u, int v) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;
    g->adj[u][v] = 0;
    if (!g->oriente)
        g->adj[v][u] = 0;
}

int graphe_matrice_existe_arc(const GrapheMatrice *g, int u, int v) {
    if (!g) return 0;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return 0;
    return g->adj[u][v] != 0;
}

int graphe_matrice_ordre(const GrapheMatrice *g) {
    if (!g) return 0;
    return g->nb_sommets;
}

int graphe_matrice_degre_sortant(const GrapheMatrice *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    for (int j = 0; j < g->nb_sommets; ++j)
        if (g->adj[v][j]) deg++;
    return deg;
}

int graphe_matrice_degre_entrant(const GrapheMatrice *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    for (int i = 0; i < g->nb_sommets; ++i)
        if (g->adj[i][v]) deg++;
    return deg;
}

int graphe_matrice_degre(const GrapheMatrice *g, int v) {
    /* For non-oriented graphs: in = out, so we can just count row */
    return graphe_matrice_degre_sortant(g, v);
}

int graphe_matrice_est_oriente(const GrapheMatrice *g) {
    if (!g) return 0;
    return g->oriente;
}
