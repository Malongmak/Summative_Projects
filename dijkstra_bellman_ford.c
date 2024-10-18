#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

// Structure to represent an edge in the graph
typedef struct edge {
    int destination;
    int weight;
    struct edge* next;
} Edge;

// Structure for the graph
typedef struct graph {
    Edge* adjList[MAX_VERTICES];
    int numVertices;
} Graph;

// Function to create a graph
Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    for (int i = 0; i < numVertices; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// Function to add an edge to the graph
void addEdge(Graph* graph, int source, int destination, int weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = destination;
    newEdge->weight = weight;
    newEdge->next = graph->adjList[source];
    graph->adjList[source] = newEdge;
}

// Function to draw the graph
void drawGraph(Graph* graph) {
    printf("\nGraph representation (adjacency list):\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d: ", i);
        Edge* edge = graph->adjList[i];
        while (edge != NULL) {
            printf("-> (%d, %d) ", edge->destination, edge->weight);
            edge = edge->next;
        }
        printf("\n");
    }
}

// Dijkstra's Algorithm
void Dijkstra(Graph* graph, int source) {
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};

    for (int i = 0; i < graph->numVertices; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    for (int i = 0; i < graph->numVertices - 1; i++) {
        int minDist = INF;
        int minIndex = -1;

        for (int j = 0; j < graph->numVertices; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                minIndex = j;
            }
        }

        if (minIndex == -1) break; // No more reachable vertices
        visited[minIndex] = 1;

        Edge* edge = graph->adjList[minIndex];
        while (edge != NULL) {
            if (!visited[edge->destination] && dist[minIndex] + edge->weight < dist[edge->destination]) {
                dist[edge->destination] = dist[minIndex] + edge->weight;
            }
            edge = edge->next;
        }
    }

    // Print the shortest distances
    printf("Dijkstra's Algorithm: Shortest distances from vertex %d:\n", source);
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Distance to vertex %d: %d\n", i, dist[i]);
    }
}

// Bellman-Ford Algorithm
void BellmanFord(Graph* graph, int source) {
    int dist[MAX_VERTICES];

    for (int i = 0; i < graph->numVertices; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    for (int i = 0; i < graph->numVertices - 1; i++) {
        for (int u = 0; u < graph->numVertices; u++) {
            Edge* edge = graph->adjList[u];
            while (edge != NULL) {
                if (dist[u] != INF && dist[u] + edge->weight < dist[edge->destination]) {
                    dist[edge->destination] = dist[u] + edge->weight;
                }
                edge = edge->next;
            }
        }
    }

    // Check for negative-weight cycles
    for (int u = 0; u < graph->numVertices; u++) {
        Edge* edge = graph->adjList[u];
        while (edge != NULL) {
            if (dist[u] != INF && dist[u] + edge->weight < dist[edge->destination]) {
                printf("Graph contains a negative weight cycle\n");
                return;
            }
            edge = edge->next;
        }
    }

    // Print the shortest distances
    printf("Bellman-Ford Algorithm: Shortest distances from vertex %d:\n", source);
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Distance to vertex %d: %d\n", i, dist[i]);
    }
}

// Free the graph's memory
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Edge* edge = graph->adjList[i];
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph);
}

// Main function
int main() {
    int vertices, edges;
    printf("Enter number of vertices: ");
    scanf("%d", &vertices);

    Graph* graph = createGraph(vertices);

    printf("Enter number of edges: ");
    scanf("%d", &edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    // Draw the graph
    drawGraph(graph);

    int startVertex;
    printf("Enter starting vertex for Dijkstra's algorithm: ");
    scanf("%d", &startVertex);
    Dijkstra(graph, startVertex);

    printf("Enter starting vertex for Bellman-Ford algorithm: ");
    scanf("%d", &startVertex);
    BellmanFord(graph, startVertex);

    freeGraph(graph);
    return 0;
}
