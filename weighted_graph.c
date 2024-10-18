#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Edge {
    int dest;
    int weight;
    struct Edge* next;
} Edge;

typedef struct Graph {
    int numVertices;
    Edge** adjList;
} Graph;

Graph* createGraph(int vertices);
void addEdge(Graph* graph, int src, int dest, int weight);
void dijkstra(Graph* graph, int startVertex);
void bellmanFord(Graph* graph, int startVertex);
void freeGraph(Graph* graph);

int main() {
    int vertices, edges, src, dest, weight, choice;

    printf("Enter number of vertices: ");
    scanf("%d", &vertices);
    
    Graph* graph = createGraph(vertices);

    printf("Enter number of edges: ");
    scanf("%d", &edges);
    for (int i = 0; i < edges; i++) {
        printf("Enter edge (src dest weight): ");
        scanf("%d %d %d", &src, &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    while (1) {
        printf("\n1. Dijkstra's Algorithm\n2. Bellman-Ford Algorithm\n3. Exit\nChoose an option: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Enter starting vertex for Dijkstra's: ");
                scanf("%d", &src);
                dijkstra(graph, src);
                break;
            case 2:
                printf("Enter starting vertex for Bellman-Ford: ");
                scanf("%d", &src);
                bellmanFord(graph, src);
                break;
            case 3:
                freeGraph(graph);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// function to create a graph - weighted graph
Graph* createGraph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjList = malloc(vertices * sizeof(Edge*));
    
    for (int i = 0; i < vertices; i++) {
        graph->adjList[i] = NULL;
    }
    
    return graph;
}

// function to add an edge to the graph - directed graph
void addEdge(Graph* graph, int src, int dest, int weight) {
    Edge* newEdge = malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = graph->adjList[src];
    graph->adjList[src] = newEdge;
}

// Dijkstra's algorithm - check for negative-weight cycles
void dijkstra(Graph* graph, int startVertex) {
    int* distances = malloc(graph->numVertices * sizeof(int));
    int* visited = malloc(graph->numVertices * sizeof(int));
    
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }

    distances[startVertex] = 0;

    for (int i = 0; i < graph->numVertices - 1; i++) {
        int minDistance = INT_MAX;
        int minVertex;

        for (int j = 0; j < graph->numVertices; j++) {
            if (!visited[j] && distances[j] < minDistance) {
                minDistance = distances[j];
                minVertex = j;
            }
        }

        visited[minVertex] = 1;

        Edge* edge = graph->adjList[minVertex];
        while (edge != NULL) {
            int newDistance = distances[minVertex] + edge->weight;
            if (newDistance < distances[edge->dest]) {
                distances[edge->dest] = newDistance;
            }
            edge = edge->next;
        }
    }

    printf("Dijkstra's Algorithm: Shortest distances from vertex %d:\n", startVertex);
    for (int i = 0; i < graph->numVertices; i++) {
        if (distances[i] == INT_MAX) {
            printf("Vertex %d: INF\n", i);
        } else {
            printf("Vertex %d: %d\n", i, distances[i]);
        }
    }

    free(distances);
    free(visited);
}

// Bellman-Ford algorithm - check for negative-weight cycles
void bellmanFord(Graph* graph, int startVertex) {
    int* distances = malloc(graph->numVertices * sizeof(int));
    
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = INT_MAX;
    }
    
    distances[startVertex] = 0;

    for (int i = 0; i < graph->numVertices - 1; i++) {
        for (int j = 0; j < graph->numVertices; j++) {
            Edge* edge = graph->adjList[j];
            while (edge != NULL) {
                if (distances[j] != INT_MAX && distances[j] + edge->weight < distances[edge->dest]) {
                    distances[edge->dest] = distances[j] + edge->weight;
                }
                edge = edge->next;
            }
        }
    }

    // check for negative-weight cycles in the graph
    for (int j = 0; j < graph->numVertices; j++) {
        Edge* edge = graph->adjList[j];
        while (edge != NULL) {
            if (distances[j] != INT_MAX && distances[j] + edge->weight < distances[edge->dest]) {
                printf("Graph contains negative weight cycle\n");
                free(distances);
                return;
            }
            edge = edge->next;
        }
    }

    printf("Bellman-Ford Algorithm: Shortest distances from vertex %d:\n", startVertex);
    for (int i = 0; i < graph->numVertices; i++) {
        if (distances[i] == INT_MAX) {
            printf("Vertex %d: INF\n", i);
        } else {
            printf("Vertex %d: %d\n", i, distances[i]);
        }
    }

    free(distances);
}

// function to free the graph
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Edge* edge = graph->adjList[i];
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph);
}