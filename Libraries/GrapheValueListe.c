/* GrapheValueListe.c */
#include <stdio.h>
#include <stdlib.h>
#include "GrapheValueListe.h"

static int redimensionner(GrapheValueListe *g, int nouvelle_capacite) {
    ArcValueListe **nouveau = (ArcValueListe **)calloc(nouvelle_capacite, sizeof(ArcValueListe *));
    if (!nouveau) return 0;
    for (int i = 0; i < g->nb_sommets; ++i)
        nouveau[i] = g->adj[i];
    free(g->adj);
    g->adj = nouveau;
    g->capacite = nouvelle_capacite;
    return 1;
}

static void supprimer_arc_une_direction(GrapheValueListe *g, int u, int v) {
    if (!g) return;
    if (u < 0 || u >= g->nb_sommets) return;
    ArcValueListe *cur = g->adj[u];
    ArcValueListe *prev = NULL;

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

GrapheValueListe *graphe_value_liste_creer(int capacite_initiale, int oriente) {
    GrapheValueListe *g = (GrapheValueListe *)malloc(sizeof(GrapheValueListe));
    if (!g) return NULL;
    if (capacite_initiale <= 0) capacite_initiale = 1;

    g->adj = (ArcValueListe **)calloc(capacite_initiale, sizeof(ArcValueListe *));
    if (!g->adj) {
        free(g);
        return NULL;
    }
    g->nb_sommets = 0;
    g->capacite   = capacite_initiale;
    g->oriente    = oriente ? 1 : 0;
    return g;
}

void graphe_value_liste_detruire(GrapheValueListe *g) {
    if (!g) return;
    for (int i = 0; i < g->nb_sommets; ++i) {
        ArcValueListe *cur = g->adj[i];
        while (cur) {
            ArcValueListe *tmp = cur;
            cur = cur->suivant;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

int graphe_value_liste_ajouter_sommet(GrapheValueListe *g) {
    if (!g) return -1;
    if (g->nb_sommets == g->capacite) {
        int nouvelle_cap = g->capacite * 2;
        if (!redimensionner(g, nouvelle_cap)) return -1;
    }
    int v = g->nb_sommets;
    g->nb_sommets++;
    g->adj[v] = NULL;
    return v;
}

void graphe_value_liste_supprimer_sommet(GrapheValueListe *g, int v) {
    if (!g) return;
    if (v < 0 || v >= g->nb_sommets) return;

    int last = g->nb_sommets - 1;

    /* 1) remove all arcs from v */
    ArcValueListe *cur = g->adj[v];
    while (cur) {
        ArcValueListe *tmp = cur;
        cur = cur->suivant;
        free(tmp);
    }
    g->adj[v] = NULL;

    /* 2) remove arcs to v and remap arcs to last -> v */
    for (int i = 0; i < g->nb_sommets; ++i) {
        if (i == v) continue;
        ArcValueListe *node = g->adj[i];
        ArcValueListe *prev = NULL;
        while (node) {
            if (node->dest == v) {
                ArcValueListe *tmp = node;
                if (prev) prev->suivant = node->suivant;
                else g->adj[i] = node->suivant;
                node = node->suivant;
                free(tmp);
            } else {
                if (node->dest == last) {
                    node->dest = v;
                }
                prev = node;
                node = node->suivant;
            }
        }
    }

    /* 3) move adjacency list from last to v */
    if (v != last) {
        g->adj[v] = g->adj[last];
        g->adj[last] = NULL;
    }

    g->nb_sommets--;
}

void graphe_value_liste_ajouter_arc(GrapheValueListe *g, int u, int v, int poids) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;
    if (poids <= 0) return; /* assume strictly positive */

    /* add u -> v */
    ArcValueListe *node = (ArcValueListe *)malloc(sizeof(ArcValueListe));
    if (!node) return;
    node->dest = v;
    node->poids = poids;
    node->suivant = g->adj[u];
    g->adj[u] = node;

    if (!g->oriente) {
        /* add v -> u */
        ArcValueListe *node2 = (ArcValueListe *)malloc(sizeof(ArcValueListe));
        if (!node2) return;
        node2->dest = u;
        node2->poids = poids;
        node2->suivant = g->adj[v];
        g->adj[v] = node2;
    }
}

void graphe_value_liste_supprimer_arc(GrapheValueListe *g, int u, int v) {
    if (!g) return;
    supprimer_arc_une_direction(g, u, v);
    if (!g->oriente)
        supprimer_arc_une_direction(g, v, u);
}

int graphe_value_liste_existe_arc(GrapheValueListe *g, int u, int v) {
    if (!g) return 0;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return 0;

    ArcValueListe *cur = g->adj[u];
    while (cur) {
        if (cur->dest == v) return 1;
        cur = cur->suivant;
    }
    return 0;
}

int graphe_value_liste_obtenir_poids(GrapheValueListe *g, int u, int v, int *poids) {
    if (!g || !poids) return 0;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return 0;
    ArcValueListe *cur = g->adj[u];
    while (cur) {
        if (cur->dest == v) {
            *poids = cur->poids;
            return 1;
        }
        cur = cur->suivant;
    }
    return 0;
}

void graphe_value_liste_modifier_poids(GrapheValueListe *g, int u, int v, int nouveau_poids) {
    if (!g) return;
    if (u < 0 || v < 0 || u >= g->nb_sommets || v >= g->nb_sommets) return;

    if (nouveau_poids <= 0) {
        graphe_value_liste_supprimer_arc(g, u, v);
        return;
    }

    /* try to find existing arc */
    ArcValueListe *cur = g->adj[u];
    while (cur) {
        if (cur->dest == v) {
            cur->poids = nouveau_poids;
            return;
        }
        cur = cur->suivant;
    }
    /* if not found, add arc */
    graphe_value_liste_ajouter_arc(g, u, v, nouveau_poids);
}

int graphe_value_liste_ordre(const GrapheValueListe *g) {
    if (!g) return 0;
    return g->nb_sommets;
}

int graphe_value_liste_degre_sortant(const GrapheValueListe *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    ArcValueListe *cur = g->adj[v];
    while (cur) {
        deg++;
        cur = cur->suivant;
    }
    return deg;
}

int graphe_value_liste_degre_entrant(const GrapheValueListe *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return 0;
    int deg = 0;
    for (int i = 0; i < g->nb_sommets; ++i) {
        ArcValueListe *cur = g->adj[i];
        while (cur) {
            if (cur->dest == v) deg++;
            cur = cur->suivant;
        }
    }
    return deg;
}

int graphe_value_liste_degre(const GrapheValueListe *g, int v) {
    return graphe_value_liste_degre_sortant(g, v);
}

int graphe_value_liste_est_oriente(const GrapheValueListe *g) {
    if (!g) return 0;
    return g->oriente;
}

ArcValueListe *graphe_value_liste_voisins(const GrapheValueListe *g, int v) {
    if (!g || v < 0 || v >= g->nb_sommets) return NULL;
    return g->adj[v];
}
