#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef enum {
    WHITE,
    GRAY,
    BLACK
} Color;

typedef struct Vertex {
    Color color;
    int b;
    int f;
    int prev;
} Vertex;

typedef struct Node {
    int vertexIndex;
    struct Node* next;
} Node;

typedef struct {
    int numOfVertices;
    Node* adjLists[MAX_VERTICES];
} Graph;

typedef struct {
    int top;
    int capacity;
    int* data;
} Stack;

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        printf("Memory allocation failed for stack\n");
        exit(EXIT_FAILURE);
    }
    stack->top = -1;
    stack->capacity = capacity;
    stack->data = (int*)malloc(capacity * sizeof(int));
    if (!stack->data) {
        printf("Memory allocation failed for data in stack\n");
        exit(EXIT_FAILURE);
    }
    return stack;
}

char isEmpty(Stack* stack) {
    return stack->top == -1;
}

void freeStack(Stack* stack) {
    free(stack->data);
    free(stack);
}

void push(Stack* stack, int p) {
    stack->data[++stack->top] = p;
}

int pop(Stack* stack) {
    return stack->data[stack->top--];
}

Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        printf("Memory allocation failed for graph\n");
        exit(EXIT_FAILURE);
    }
    graph->numOfVertices = vertices;
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed for edge: (%d -> %d)\n", src, dest);
        exit(EXIT_FAILURE);
    }
    newNode->vertexIndex = dest;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->numOfVertices; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph);
}

int counter;
int isCyclic;

void recursiveDFS(Graph* graph, Stack* stack, Vertex vertices[], int v) {
    vertices[v].color = GRAY;
    vertices[v].b = counter++;

    for (Node* temp = graph->adjLists[v]; temp; temp = temp->next) {
        int u = temp->vertexIndex;
        if (vertices[u].color == WHITE) {
            vertices[u].prev = v;
            recursiveDFS(graph, stack, vertices, u);
        }
        if (vertices[u].color == GRAY) {
            isCyclic = 1;
            return;
        }
    }
    vertices[v].color = BLACK;
    vertices[v].f = counter++;
    push(stack, v);
}

void printTopologicalSort(Stack* stack) {
    printf("Topological order of the vertices:\n");
    while (!isEmpty(stack)) {
        printf("%d ", pop(stack));
    }
    printf("\n");
}

void topologicalSort(Graph* graph, Vertex vertices[]) {
    Stack* stack = createStack(graph->numOfVertices);
    for (int i = 0; i < graph->numOfVertices; i++) {
        vertices[i].color = WHITE;
        vertices[i].prev = -1;
    }
    counter = 0;
    isCyclic = 0;
    for (int i = 0; i < graph->numOfVertices; i++) {
        if (vertices[i].color == WHITE) {
            recursiveDFS(graph, stack, vertices, i);
        }
    }
    if (isCyclic) {
        printf("Graph is cyclic, topological sort is not possible\n");
        freeStack(stack);
        return;
    }

    printTopologicalSort(stack);
    freeStack(stack);
}

int main() {
    Graph* graph = createGraph(9);
    Vertex* vertices = (Vertex*)malloc((graph->numOfVertices) * sizeof(Vertex));
    if (!vertices) {
        printf("Memory allocation failed for vertices\n");
        freeGraph(graph);
        exit(EXIT_FAILURE);
    }

    addEdge(graph, 2, 1);
    addEdge(graph, 1, 0);
    addEdge(graph, 0, 6);
    addEdge(graph, 0, 8);
    addEdge(graph, 3, 7);
    addEdge(graph, 4, 0);
    addEdge(graph, 4, 1);
    addEdge(graph, 4, 5);
    addEdge(graph, 4, 8);
    addEdge(graph, 6, 8);
    addEdge(graph, 7, 8);
    addEdge(graph, 7, 6);


    topologicalSort(graph, vertices);

    freeGraph(graph);
    free(vertices);

    return 0;
}