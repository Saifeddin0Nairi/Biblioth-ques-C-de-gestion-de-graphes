
/* main.c */
#include <stdio.h>
#include <stdlib.h>

#include "GrapheMatrice.h"
#include "GrapheListe.h"
#include "GrapheValueMatrice.h"
#include "GrapheValueListe.h"

/* Algorithms */
void dfs_composantes_connexes(GrapheListe *g);
void bfs_distances(GrapheListe *g, int source, int *dist);
int  graphe_liste_est_connexe(GrapheListe *g);
int  graphe_liste_est_fortement_connexe(GrapheListe *g);
void dijkstra(GrapheValueListe *g, int source, int *dist, int *pred);

static void test_graphe_liste() {
    printf("=== Test GrapheListe (unweighted, undirected) ===\n");
    GrapheListe *g = graphe_liste_creer(4, 0); /* 0 = non oriented */

    int v0 = graphe_liste_ajouter_sommet(g);
    int v1 = graphe_liste_ajouter_sommet(g);
    int v2 = graphe_liste_ajouter_sommet(g);
    int v3 = graphe_liste_ajouter_sommet(g);

    graphe_liste_ajouter_arc(g, v0, v1);
    graphe_liste_ajouter_arc(g, v1, v2);
    graphe_liste_ajouter_arc(g, v2, v3);

    printf("Order = %d\n", graphe_liste_ordre(g));
    for (int i = 0; i < graphe_liste_ordre(g); ++i) {
        printf("Degree of %d = %d\n", i, graphe_liste_degre(g, i));
    }

    printf("DFS connected components:\n");
    dfs_composantes_connexes(g);

    int n = graphe_liste_ordre(g);
    int *dist = (int *)malloc(n * sizeof(int));
    bfs_distances(g, 0, dist);
    printf("BFS distances from 0:\n");
    for (int i = 0; i < n; ++i) {
        printf("dist[0 -> %d] = %d\n", i, dist[i]);
    }

    printf("Graph is connected? %s\n", graphe_liste_est_connexe(g) ? "YES" : "NO");

    free(dist);
    graphe_liste_detruire(g);
}

static void test_graphe_matrice() {
    printf("\n=== Test GrapheMatrice (unweighted, directed) ===\n");
    GrapheMatrice *g = graphe_matrice_creer(3, 1); /* 1 = oriented */

    int v0 = graphe_matrice_ajouter_sommet(g);
    int v1 = graphe_matrice_ajouter_sommet(g);
    int v2 = graphe_matrice_ajouter_sommet(g);

    graphe_matrice_ajouter_arc(g, v0, v1);
    graphe_matrice_ajouter_arc(g, v1, v2);

    printf("Order = %d\n", graphe_matrice_ordre(g));
    printf("Edge 0->1 exists? %s\n",
           graphe_matrice_existe_arc(g, 0, 1) ? "YES" : "NO");
    printf("Edge 1->0 exists? %s\n",
           graphe_matrice_existe_arc(g, 1, 0) ? "YES" : "NO");

    for (int i = 0; i < graphe_matrice_ordre(g); ++i) {
        printf("Out degree of %d = %d, In degree = %d\n",
               i,
               graphe_matrice_degre_sortant(g, i),
               graphe_matrice_degre_entrant(g, i));
    }

    graphe_matrice_detruire(g);
}

static void test_graphe_value_liste() {
    printf("\n=== Test GrapheValueListe + Dijkstra ===\n");
    GrapheValueListe *g = graphe_value_liste_creer(5, 1); /* directed valued graph */

    int v0 = graphe_value_liste_ajouter_sommet(g);
    int v1 = graphe_value_liste_ajouter_sommet(g);
    int v2 = graphe_value_liste_ajouter_sommet(g);
    int v3 = graphe_value_liste_ajouter_sommet(g);
    int v4 = graphe_value_liste_ajouter_sommet(g);

    graphe_value_liste_ajouter_arc(g, v0, v1, 10);
    graphe_value_liste_ajouter_arc(g, v0, v2, 3);
    graphe_value_liste_ajouter_arc(g, v2, v1, 1);
    graphe_value_liste_ajouter_arc(g, v1, v3, 2);
    graphe_value_liste_ajouter_arc(g, v2, v3, 8);
    graphe_value_liste_ajouter_arc(g, v3, v4, 7);

    int n = graphe_value_liste_ordre(g);
    int *dist = (int *)malloc(n * sizeof(int));
    int *pred = (int *)malloc(n * sizeof(int));

    dijkstra(g, 0, dist, pred);

    printf("Shortest distances from 0:\n");
    for (int i = 0; i < n; ++i) {
        if (dist[i] == 1000000000)
            printf("0 -> %d : unreachable\n", i);
        else
            printf("0 -> %d : %d\n", i, dist[i]);
    }

    free(dist);
    free(pred);
    graphe_value_liste_detruire(g);
}

static void test_graphe_value_matrice() {
    printf("\n=== Test GrapheValueMatrice ===\n");
    GrapheValueMatrice *g = graphe_value_matrice_creer(3, 0); /* undirected weighted */

    int v0 = graphe_value_matrice_ajouter_sommet(g);
    int v1 = graphe_value_matrice_ajouter_sommet(g);
    int v2 = graphe_value_matrice_ajouter_sommet(g);

    graphe_value_matrice_ajouter_arc(g, v0, v1, 5);
    graphe_value_matrice_ajouter_arc(g, v1, v2, 2);

    printf("Order = %d\n", graphe_value_matrice_ordre(g));
    for (int i = 0; i < graphe_value_matrice_ordre(g); ++i) {
        for (int j = 0; j < graphe_value_matrice_ordre(g); ++j) {
            int w = graphe_value_matrice_obtenir_poids(g, i, j);
            if (w > 0)
                printf("Edge %d-%d weight=%d\n", i, j, w);
        }
    }

    graphe_value_matrice_detruire(g);
}

int main(void) {
    test_graphe_liste();
    test_graphe_matrice();
    test_graphe_value_liste();
    test_graphe_value_matrice();
    return 0;
}
