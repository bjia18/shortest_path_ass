#define _CRT_SECURE_NO_WARNINGS

#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#define airports_txt "small-airports.txt"
#define dist_txt "small-dists.txt"
#define HASH_SIZE 26*26*26

struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};

// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

struct ListGraph{
    int V;
    struct AdjList* array;
};

struct MinHeapNode{
    int  v;
    int dist;
};

// Structure to represent a min heap
struct MinHeap{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

struct Edge{
        int weight, source, destination;
};

struct Graph{
        int V, E;
        struct Edge* edge;
};

extern int arr[HASH_SIZE];
extern char map[][4];
extern int hashed;

void commands();
void airports();
void help();
void shortest_distance(char *port1, char *port2);
void dijkstra(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num);
void bellman_ford(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num);

int simple_hash(char *str);
void insert(char *str, int pos);

struct Graph *createGraph(int V, int E);

#endif
