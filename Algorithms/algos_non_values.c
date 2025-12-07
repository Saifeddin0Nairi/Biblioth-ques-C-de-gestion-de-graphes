/* algos_non_values.c DFS, BFS, connectivity */
#include <stdio.h>
#include <stdlib.h>
#include "GrapheListe.h"

/* ---------- DFS (recursive) ---------- */

static void dfs_rec(GrapheListe *g, int v, int *visite) {
    visite[v] = 1;
    printf("%d ", v);

    ArcListe *voisin = graphe_liste_voisins(g, v);
    while (voisin) {
        int u = voisin->dest;
        if (!visite[u]) {
            dfs_rec(g, u, visite);
        }
        voisin = voisin->suivant;
    }
}

/* Explore all vertices and print connected components (for non-oriented graphs) */
void dfs_composantes_connexes(GrapheListe *g) {
    int n = graphe_liste_ordre(g);
    int *visite = (int *)calloc(n, sizeof(int));
    if (!visite) return;

    int num_comp = 0;
    for (int v = 0; v < n; ++v) {
        if (!visite[v]) {
            num_comp++;
            printf("Component %d: ", num_comp);
            dfs_rec(g, v, visite);
            printf("\n");
        }
    }

    free(visite);
}

/* ---------- BFS and distances ---------- */

void bfs_distances(GrapheListe *g, int source, int *dist) {
    int n = graphe_liste_ordre(g);
    if (source < 0 || source >= n) return;

    /* Initialize distances to -1 (unreachable) */
    for (int i = 0; i < n; ++i) dist[i] = -1;
    dist[source] = 0;

    int *queue = (int *)malloc(n * sizeof(int));
    if (!queue) return;

    int head = 0, tail = 0;
    queue[tail++] = source;

    while (head < tail) {
        int v = queue[head++];

        ArcListe *voisin = graphe_liste_voisins(g, v);
        while (voisin) {
            int u = voisin->dest;
            if (dist[u] == -1) {
                dist[u] = dist[v] + 1;
                queue[tail++] = u;
            }
            voisin = voisin->suivant;
        }
    }

    free(queue);
}

/* ---------- Connectivity ---------- */

int graphe_liste_est_connexe(GrapheListe *g) {
    int n = graphe_liste_ordre(g);
    if (n == 0) return 1;

    int *dist = (int *)malloc(n * sizeof(int));
    if (!dist) return 0;

    bfs_distances(g, 0, dist);

    for (int i = 0; i < n; ++i) {
        if (dist[i] == -1) {
            free(dist);
            return 0;
        }
    }
    free(dist);
    return 1;
}

/* Strong connectivity for oriented graph:
   We simply run BFS from every vertex and check that all others are reachable.
   This is O(V * (V+E)) but easy to understand. */
int graphe_liste_est_fortement_connexe(GrapheListe *g) {
    int n = graphe_liste_ordre(g);
    if (n == 0) return 1;

    int *dist = (int *)malloc(n * sizeof(int));
    if (!dist) return 0;

    for (int s = 0; s < n; ++s) {
        bfs_distances(g, s, dist);
        for (int i = 0; i < n; ++i) {
            if (dist[i] == -1) {
                free(dist);
                return 0;
            }
        }
    }

    free(dist);
    return 1;
}
