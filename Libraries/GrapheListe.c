/* GrapheListe.c */
#include <stdio.h>
#include <stdlib.h>
#include "GrapheListe.h"

static int redimensionner(GrapheListe *g, int nouvelle_capacite) {
    ArcListe **nouveau = (ArcListe **)calloc(nouvelle_capacite, sizeof(ArcListe *));
    if (!nouveau) return 0;
    for (int i = 0; i < g->nb_sommets; ++i)
        nouveau[i] = g->adj[i];
    free(g->adj);
    g->adj = nouveau;
    g->capacite = nouvelle_capacite;
    return 1;
}

static void supprimer_arc_une_direction(GrapheListe *g, int u, int v) {
    if (!g) return;
    if (u < 0 || u >= g->nb_sommets) return;
    ArcListe *cur = g->adj[u];
    ArcListe *prev = NULL;

    while (cur) {
        if (cur->dest == v) {
            if (prev) prev->suivant = cur->suivant;
            else g->adj[u] = cur->suivant;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->suivant;
    }
}

/* PUBLIC API */

GrapheListe *graphe_liste_creer(int capacite_initiale, int oriente) {
    GrapheListe *g = (GrapheListe *)malloc(sizeof(GrapheListe));
    if (!g) return NULL;
    if (capacite_initiale <= 0) capacite_initiale = 1;

    g->adj = (ArcListe **)calloc(capacite_initiale, sizeof(ArcListe *));
    if (!g->adj) {
        free(g);
        return NULL;
    }
    g->nb_sommets = 0;
    g->capacite   = capacite_initiale;
    g->oriente    = oriente ? 1 : 0;
    return g;
}

void graphe_liste_detruire(GrapheListe *g) {
    if (!g) return;
    for (int i = 0; i < g->nb_sommets; ++i) {
        ArcListe *cur = g->adj[i];
        while (cur) {
            ArcListe *tmp = cur;
            cur = cur->suivant;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

int graphe_liste_ajouter_sommet(GrapheListe *g) {
    if (!g) return -1;
    if (g->nb_sommets == g->capacite) {
        int nouvelle_capacite = g->capacite * 2;
        if (!redimensionner(g, nouvelle_capacite)) return -1;
    }
    int v = g->nb_sommets;
    g->nb_sommets++;
    g->adj[v] = NULL;
    return v;
}

void graphe_liste_supprimer_sommet(GrapheListe *g, int v) {
    if (!g) return;
    if (v < 0 || v >= g->nb_sommets) return;

    int last = g->nb_sommets - 1;

    /* 1) Remove all arcs from v (we delete that vertex) */
    ArcListe *cur = g->adj[v];
    while (cur) {
        ArcListe *tmp = cur;
        cur = cur->suivant;
        free(tmp);
    }
    g->adj[v] = NULL;

    /* 2) For each vertex, remove arcs to v and remap arcs to 'last' -> 'v' */
    for (int i = 0; i < g->nb_sommets; ++i) {
        if (i == v) continue;
        ArcListe *node = g->adj[i];
        ArcListe *prev = NULL;
        while (node) {
            if (node->dest == v) {
                /* Remove arcs going to the deleted vertex v */
                ArcListe *tmp = node;
                if (prev) prev->suivant = node->suivant;
                else g->adj[i] = node->suivant;
                node = node->suivant;
                free(tmp);
            } else {
                if (node->dest == last) {
                    /* Remap arcs pointing to 'last' to now point to 'v' */
                    node->dest = v;
                }
                prev = node;
                node = node->suivant;
            }
        }
    }

    /* 3) Move adjacency list of last vertex to position v */
    if (v != last) {
        g->adj[v] = g->adj[last];
        g->adj[last] = NULL;
    }

    g->nb_sommets--;
}

void graphe_liste_ajouter_arc(GrapheListe *g, int u, int v) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;

    /* add u -> v */
    ArcListe *node = (ArcListe *)malloc(sizeof(ArcListe));
    if (!node) return;
    node->dest = v;
    node->suivant = g->adj[u];
    g->adj[u] = node;

    if (!g->oriente) {
        /* add v -> u */
        ArcListe *node2 = (ArcListe *)malloc(sizeof(ArcListe));
        if (!node2) return;
        node2->dest = u;
        node2->suivant = g->adj[v];
        g->adj[v] = node2;
    }
}

void graphe_liste_supprimer_arc(GrapheListe *g, int u, int v) {
    if (!g) return;
    supprimer_arc_une_direction(g, u, v);
    if (!g->oriente)
        supprimer_arc_une_direction(g, v, u);
}

int graphe_liste_existe_arc(GrapheListe *g, int u, int v) {
    if (!g) return 0;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return 0;

    ArcListe *cur = g->adj[u];
    while (cur) {
        if (cur->dest == v) return 1;
        cur = cur->suivant;
    }
    return 0;
}

int graphe_liste_ordre(const GrapheListe *g) {
    if (!g) return 0;
    return g->nb_sommets;
}

int graphe_liste_degre_sortant(const GrapheListe *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    ArcListe *cur = g->adj[v];
    while (cur) {
        deg++;
        cur = cur->suivant;
    }
    return deg;
}

int graphe_liste_degre_entrant(const GrapheListe *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    for (int i = 0; i < g->nb_sommets; ++i) {
        ArcListe *cur = g->adj[i];
        while (cur) {
            if (cur->dest == v) deg++;
            cur = cur->suivant;
        }
    }
    return deg;
}

int graphe_liste_degre(const GrapheListe *g, int v) {
    /* For non-oriented graphs: total degree = out-degree = in-degree */
    return graphe_liste_degre_sortant(g, v);
}

int graphe_liste_est_oriente(const GrapheListe *g) {
    if (!g) return 0;
    return g->oriente;
}

ArcListe *graphe_liste_voisins(const GrapheListe *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return NULL;
    return g->adj[v];
}
