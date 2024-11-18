#include <stdio.h>

#define INF 99999
#define N 4

void floydWarshall(int dist[N][N]) {
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

void printSolution(int dist[N][N]) {
    printf("Shortest distances between every pair of cities:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (dist[i][j] == INF)
                printf("%4s", "INF");
            else
                printf("%4d", dist[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int dist[N][N] = {
        {0, INF, 2, INF},
        {6, 0, 5, INF},
        {INF, INF, 0, 4},
        {INF, 7, INF, 0}
    };

    floydWarshall(dist);

    printSolution(dist);

    printf("\nShortest distance from City A to City C: %d\n", dist[0][2]);
    printf("Shortest distance from City B to City D: %d\n", dist[1][3]);

    return 0;
}
