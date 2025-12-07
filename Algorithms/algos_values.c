/* algos_values.c */
#include <stdio.h>
#include <stdlib.h>
#include "GrapheValueListe.h"

#define INF 1000000000

/* Dijkstra: shortest paths from "source" in a graph with non-negative weights.
   dist[i] will contain the shortest distance from source to i,
   pred[i] will contain the predecessor of i on the shortest path (or -1). */

void dijkstra(GrapheValueListe *g, int source, int *dist, int *pred) {
    int n = graphe_value_liste_ordre(g);
    if (source < 0 || source >= n) return;

    int *vu = (int *)calloc(n, sizeof(int));
    if (!vu) return;

    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        pred[i] = -1;
    }
    dist[source] = 0;

    for (int k = 0; k < n; ++k) {
        /* Select unvisited vertex with minimal distance */
        int u = -1;
        int best = INF;
        for (int i = 0; i < n; ++i) {
            if (!vu[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break; /* remaining vertices are unreachable */

        vu[u] = 1;

        /* Relax edges u -> v */
        ArcValueListe *arc = graphe_value_liste_voisins(g, u);
        while (arc) {
            int v = arc->dest;
            int w = arc->poids;
            if (!vu[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pred[v] = u;
            }
            arc = arc->suivant;
        }
    }

    free(vu);
}
