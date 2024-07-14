#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3
#define MAX_HEAP_SIZE 1000000

typedef struct Node {
    int board[N][N];
    int g, h, f;
    struct Node* parent;
} Node;

typedef struct {
    Node* data[MAX_HEAP_SIZE];
    int size;
} MinHeap;

MinHeap* createMinHeap() {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    return heap;
}

void swap(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int index) {
    if (index && heap->data[(index - 1) / 2]->f > heap->data[index]->f) {
        swap(&heap->data[index], &heap->data[(index - 1) / 2]);
        heapifyUp(heap, (index - 1) / 2);
    }
}

void heapifyDown(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->data[left]->f < heap->data[smallest]->f) {
        smallest = left;
    }

    if (right < heap->size && heap->data[right]->f < heap->data[smallest]->f) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&heap->data[smallest], &heap->data[index]);
        heapifyDown(heap, smallest);
    }
}

void push(MinHeap* heap, Node* node) {
    if (heap->size == MAX_HEAP_SIZE) {
        printf("Heap overflow\n");
        return;
    }

    heap->data[heap->size] = node;
    heapifyUp(heap, heap->size);
    heap->size++;
}

Node* pop(MinHeap* heap) {
    if (heap->size <= 0) {
        return NULL;
    }

    Node* root = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapifyDown(heap, 0);

    return root;
}

int isGoal(int board[N][N]) {
    int goal[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] != goal[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

int manhattanDistance(int board[N][N]) {
    int goal[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    int distance = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] != 0) {
                for (int x = 0; x < N; x++) {
                    for (int y = 0; y < N; y++) {
                        if (board[i][j] == goal[x][y]) {
                            distance += abs(i - x) + abs(j - y);
                        }
                    }
                }
            }
        }
    }
    return distance;
}

Node* createNode(int board[N][N], int g, int h, Node* parent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    memcpy(newNode->board, board, sizeof(newNode->board));
    newNode->g = g;
    newNode->h = h;
    newNode->f = g + h;
    newNode->parent = parent;
    return newNode;
}

void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printSolution(Node* node) {
    if (node == NULL) {
        return;
    }
    printSolution(node->parent);
    printBoard(node->board);
}

void solve(int start[N][N]) {
    MinHeap* openSet = createMinHeap();
    Node* startNode = createNode(start, 0, manhattanDistance(start), NULL);
    push(openSet, startNode);

    while (openSet->size > 0) {
        Node* current = pop(openSet);

        if (isGoal(current->board)) {
            printf("Solution found:\n");
            printSolution(current);
            return;
        }

        int blankX, blankY;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (current->board[i][j] == 0) {
                    blankX = i;
                    blankY = j;
                }
            }
        }

        int moves[4][2] = {
            {blankX - 1, blankY}, {blankX + 1, blankY},
            {blankX, blankY - 1}, {blankX, blankY + 1}
        };

        for (int i = 0; i < 4; i++) {
            int newX = moves[i][0];
            int newY = moves[i][1];

            if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
                int newBoard[N][N];
                memcpy(newBoard, current->board, sizeof(newBoard));
                newBoard[blankX][blankY] = newBoard[newX][newY];
                newBoard[newX][newY] = 0;

                int newG = current->g + 1;
                int newH = manhattanDistance(newBoard);
                Node* newNode = createNode(newBoard, newG, newH, current);

                push(openSet, newNode);
            }
        }
    }

    printf("No solution found.\n");
}

int main() {
    int start[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8}
    };

    solve(start);

    return 0;
}
