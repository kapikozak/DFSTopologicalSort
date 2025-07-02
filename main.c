#include <stdio.h>
#include <stdlib.h>

typedef enum {
    white,
    grey,
    black,
} Colour;

struct ListNode {
    int val;
    struct ListNode* next;
};

struct Graph {
    int nv;
    struct ListNode** al;
};

void initGraph(struct Graph* g, int numOfVertices) {
    g->nv = numOfVertices;
    g->al = (struct ListNode**)malloc((g->nv) * sizeof(struct ListNode*));
    if (g->al) {
        for (int i = 0; i < g->nv; i++) {
            g->al[i] = NULL;
        }
    } else {
        fprintf(stderr, "Memory allocation failed for Graph.\n");
        exit(EXIT_FAILURE);
    }
} /*initGraph*/

void addEdge(struct Graph* g, int v1, int v2) {
    struct ListNode* edge = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (edge) {
        edge->val = v2;
        edge->next = g->al[v1];
        g->al[v1] = edge;
    } else {
        fprintf(stderr, "Memory allocation failed for edge: (%d, %d).\n", v1, v2);
        exit(EXIT_FAILURE);
    }
} /*addEdge*/

void freeGraph(struct Graph* g) {
    for (int i = 0; i < g->nv; i++) {
        while (g->al[i]) {
            struct ListNode *tmp = g->al[i];
            g->al[i] = g->al[i]->next;
            free(tmp);
        }
    }
    free(g->al);
} /*freeGraph*/

typedef struct {
    int top;
    int* data;
} Stack;

void initStack(Stack* s, int capacity) {
    s->top = -1;
    s->data = (int*)malloc(capacity * sizeof(int));
    if (!s->data) {
        fprintf(stderr, "Memory allocation failed for Stack.\n");
        exit(EXIT_FAILURE);
    }
} /*initStack*/

void push(Stack* s, int elem) {
    s->data[++s->top] = elem;
} /*push*/

void pop(Stack* s, int* elem) {
    *elem = s->data[s->top--];
} /*pop*/

int isEmpty(Stack* s) {
    return s->top == -1;
} /*isEmpty*/

void freeStack(Stack* s) {
    free(s->data);
    s->data = NULL;
} /*freeStack*/

int isCycled;

void rDFS(struct Graph* g, Stack* s, Colour* colours, int v) {
    int u;
    colours[v] = grey;
    struct ListNode* curr = g->al[v];
    while (curr) {
        u = curr->val;
        if (colours[u] == white) {
            rDFS(g, s, colours, u);
        } else if (colours[u] == grey){
            isCycled = 1;
        }
        curr = curr->next;
    }
    colours[v] = black;
    push(s, v);
} /*rDFS*/

void DFS(struct Graph* g, Stack* s) {
    Colour* colours = (Colour*)calloc(g->nv, sizeof(Colour));
    if (colours) {
        isCycled = 0;
        for (int i = 0; i < g->nv; i++) {
            if (colours[i] == white) {
                rDFS(g, s, colours, i);
            }
        }
        free(colours);
    } else {
        printf("Memory allocation failed for colours array.\n");
        exit(EXIT_FAILURE);
    }
} /*DFS*/

void printTopologicalSort(Stack* s) {
    if (isCycled) {
        printf("Graph is cycled, topological sort cannot be performed.\n");
    } else {
        int idx;
        printf("Topological order of the vertices:\n");
        while (!isEmpty(s)) {
            pop(s, &idx);
            printf("%d", idx);
            if (!isEmpty(s)) {
                printf(" -> ");
            }
        }
        printf("\n");
    }
} /*printTopologicalSort*/

int main() {
    struct Graph g;
    initGraph(&g, 9);

    addEdge(&g, 2, 1);
    addEdge(&g, 1, 0);
    addEdge(&g, 0, 6);
    addEdge(&g, 0, 8);
    addEdge(&g, 6, 8);
    addEdge(&g, 3, 7);
    addEdge(&g, 7, 6);
    addEdge(&g, 7, 8);
    addEdge(&g, 4, 5);
    addEdge(&g, 4, 0);
    addEdge(&g, 4, 8);
    addEdge(&g, 4, 1);

    Stack s;

    initStack(&s, g.nv);
    DFS(&g, &s);

    printTopologicalSort(&s);

    freeStack(&s);
    freeGraph(&g);
    return 0;
}
