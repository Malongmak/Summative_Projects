#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_NODES 256

// Node structure for Huffman tree
typedef struct Node {
    char character;
    int frequency;
    struct Node *left;
    struct Node *right;
} Node;

// Priority queue structure for Huffman coding
typedef struct PriorityQueue {
    Node *array[MAX_TREE_NODES];
    int size;
} PriorityQueue;

// Function prototypes for Huffman coding
Node* createNode(char character, int frequency);
PriorityQueue* createPriorityQueue();
void insert(PriorityQueue *pq, Node *node);
Node* extractMin(PriorityQueue *pq);
void buildHuffmanTree(PriorityQueue *pq);
void generateCodes(Node *root, char codes[MAX_TREE_NODES][MAX_TREE_NODES], char *code, int depth);
void compressFile(const char *inputFile, const char *outputFile, char codes[MAX_TREE_NODES][MAX_TREE_NODES]);
void decompressFile(const char *inputFile, const char *outputFile, Node *root);
void freeTree(Node *root);
void countFrequency(const char *inputFile, int frequency[MAX_TREE_NODES]);
int parent(int index);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];
    const char *outputFile = argv[2];

    int frequency[MAX_TREE_NODES] = {0};
    countFrequency(inputFile, frequency);

    PriorityQueue *pq = createPriorityQueue();
    for (int i = 0; i < MAX_TREE_NODES; i++) {
        if (frequency[i] > 0) {
            insert(pq, createNode(i, frequency[i]));
        }
    }

    buildHuffmanTree(pq);
    Node *root = extractMin(pq);

    char codes[MAX_TREE_NODES][MAX_TREE_NODES] = {0};
    generateCodes(root, codes, "", 0);

    compressFile(inputFile, outputFile, codes);
    decompressFile(outputFile, "decompressed.txt", root);

    // Clean up memory
    freeTree(root);
    free(pq);

    return 0;
}

// Create a new node for the Huffman tree
Node* createNode(char character, int frequency) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Create a priority queue
PriorityQueue* createPriorityQueue() {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    return pq;
}

// Calculate parent index in the priority queue
int parent(int index) {
    return (index - 1) / 2;
}

// Insert a node into the priority queue
void insert(PriorityQueue *pq, Node *node) {
    pq->array[pq->size++] = node;
    int index = pq->size - 1;

    // Bubble up the new node to maintain heap property
    while (index > 0 && pq->array[parent(index)]->frequency > pq->array[index]->frequency) {
        Node *temp = pq->array[parent(index)];
        pq->array[parent(index)] = pq->array[index];
        pq->array[index] = temp;
        index = parent(index);
    }
}

// Extract the minimum node from the priority queue
Node* extractMin(PriorityQueue *pq) {
    Node *minNode = pq->array[0];
    pq->array[0] = pq->array[--pq->size];

    // Bubble down the root node to maintain heap property
    int index = 0;
    while (index < pq->size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < pq->size && pq->array[left]->frequency < pq->array[smallest]->frequency) {
            smallest = left;
        }
        if (right < pq->size && pq->array[right]->frequency < pq->array[smallest]->frequency) {
            smallest = right;
        }
        if (smallest == index) break;

        Node *temp = pq->array[index];
        pq->array[index] = pq->array[smallest];
        pq->array[smallest] = temp;
        index = smallest;
    }

    return minNode;
}

// Build the Huffman tree from the priority queue
void buildHuffmanTree(PriorityQueue *pq) {
    while (pq->size > 1) {
        Node *left = extractMin(pq);
        Node *right = extractMin(pq);
        Node *parent = createNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        insert(pq, parent);
    }
}

// Generate Huffman codes for each character
void generateCodes(Node *root, char codes[MAX_TREE_NODES][MAX_TREE_NODES], char *code, int depth) {
    if (root->left == NULL && root->right == NULL) {
        code[depth] = '\0';
        strcpy(codes[(unsigned char)root->character], code);
        return;
    }

    if (root->left) {
        code[depth] = '0';
        generateCodes(root->left, codes, code, depth + 1);
    }
    if (root->right) {
        code[depth] = '1';
        generateCodes(root->right, codes, code, depth + 1);
    }
}

// Compress the input file using Huffman codes
void compressFile(const char *inputFile, const char *outputFile, char codes[MAX_TREE_NODES][MAX_TREE_NODES]) {
    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "wb");
    if (!in || !out) {
        perror("Failed to open file");
        return;
    }

    int c;
    while ((c = fgetc(in)) != EOF) {
        fputs(codes[(unsigned char)c], out);
    }

    fclose(in);
    fclose(out);
}

// Decompress the output file using the Huffman tree
void decompressFile(const char *inputFile, const char *outputFile, Node *root) {
    FILE *in = fopen(inputFile, "rb");
    FILE *out = fopen(outputFile, "w");
    if (!in || !out) {
        perror("Failed to open file");
        return;
    }

    Node *current = root;
    int bit;
    while ((bit = fgetc(in)) != EOF) {
        current = (bit == '0') ? current->left : current->right;

        // When we reach a leaf node, output the character
        if (current->left == NULL && current->right == NULL) {
            fputc(current->character, out);
            current = root; // Go back to the root
        }
    }

    fclose(in);
    fclose(out);
}

// Free the memory allocated for the Huffman tree
void freeTree(Node *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// Count the frequency of each character in the input file
void countFrequency(const char *inputFile, int frequency[MAX_TREE_NODES]) {
    FILE *file = fopen(inputFile, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        frequency[c]++;
    }

    fclose(file);
}
