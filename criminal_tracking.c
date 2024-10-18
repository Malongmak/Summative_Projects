#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PEOPLE 100
#define NAME_LENGTH 50
#define PHONE_LENGTH 15

// Structure for a person
typedef struct Person {
    char name[NAME_LENGTH];
    char phone[PHONE_LENGTH];
} Person;

// Structure for a node in the adjacency list
typedef struct Node {
    int personIndex;
    struct Node* next;
} Node;

// structure for the graph 
typedef struct Graph {
    Person people[MAX_PEOPLE];
    Node* adjList[MAX_PEOPLE];
    int numPeople;
} Graph;

// A new graph is created here
Graph* createGraph(int numPeople) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numPeople = numPeople;
    for (int i = 0; i < numPeople; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// an edge is added to the graph here
void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->personIndex = dest;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    newNode->personIndex = src;
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

// Trace the contacts using BFS functions
void BFS(Graph* graph, int start, int levels[MAX_PEOPLE]) {
    bool visited[MAX_PEOPLE] = {false};
    int queue[MAX_PEOPLE];
    int front = 0, rear = 0;

    visited[start] = true;
    queue[rear++] = start;
    levels[start] = 0;

    while (front < rear) {
        int current = queue[front++];

        Node* temp = graph->adjList[current];
        while (temp != NULL) {
            int neighbor = temp->personIndex;

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue[rear++] = neighbor;
                levels[neighbor] = levels[current] + 1;
            }
            temp = temp->next;
        }
    }
}

// Print this contacts by level
void printContactsByLevel(Graph* graph, int levels[MAX_PEOPLE]) {
    printf("Contacts by level:\n");
    for (int level = 0; level < MAX_PEOPLE; level++) {
        printf("Level %d: ", level);
        for (int i = 0; i < graph->numPeople; i++) {
            if (levels[i] == level) {
                printf("%s (Phone: %s) ", graph->people[i].name, graph->people[i].phone);
            }
        }
        printf("\n");
    }
}

int main() {
    Graph* graph = createGraph(10);
    
    // Add people and their contacts here
    strcpy(graph->people[0].name, "Samuel");
    strcpy(graph->people[0].phone, "34343434");
    strcpy(graph->people[1].name, "Pius");
    strcpy(graph->people[1].phone, "232323");
    strcpy(graph->people[2].name, "Pascal");
    strcpy(graph->people[2].phone, "33335");
    strcpy(graph->people[3].name, "Magot");
    strcpy(graph->people[3].phone, "4444");
    strcpy(graph->people[4].name, "David");
    strcpy(graph->people[4].phone, "555");
    strcpy(graph->people[5].name, "Joshua");
    strcpy(graph->people[5].phone, "666");
    strcpy(graph->people[6].name, "James");
    strcpy(graph->people[6].phone, "76584");
    strcpy(graph->people[7].name, "Maria");
    strcpy(graph->people[7].phone, "89012");
    strcpy(graph->people[8].name, "Jane");
    strcpy(graph->people[8].phone, "345716");
    strcpy(graph->people[9].name, "Joseph");
    strcpy(graph->people[9].phone, "234567");
    
    // add the edges here
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2); 
    addEdge(graph, 1, 3); 
    addEdge(graph, 1, 4); 
    addEdge(graph, 2, 5); 
    addEdge(graph, 3, 6); 
    addEdge(graph, 4, 7); 
    addEdge(graph, 5, 8); 
    addEdge(graph, 6, 9); 

    int source;
    printf("Enter the index of the criminal (0 to %d): ", graph->numPeople - 1);
    scanf("%d", &source);

    int levels[MAX_PEOPLE];
    memset(levels, -1, sizeof(levels)); // -1 indicates unvisited

    BFS(graph, source, levels);
    printContactsByLevel(graph, levels);

    // free the memory here

    return 0;
}
