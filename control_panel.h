#define _CRT_SECURE_NO_WARNINGS

#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

#define airports_txt "small-airports.txt"
#define dist_txt "small-dists.txt"
#define HASH_SIZE 500 

struct ListGraph{
    int V;
    struct AdjList* array;
};

struct Edge{
        int weight, source, destination;
};

struct Graph{
        int V, E;
        struct Edge* edge;
};

extern char arr[HASH_SIZE][4];
extern int hashed;

void commands();
void airports();
void help();
void shortest_distance(char *port1, char *port2);
void dijkstra(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num);
void bellman_ford(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num);

int simple_hash(char *str);
int search(char *str, int pos);
void insert(char *str, int pos);

struct Graph *createGraph(int V, int E);

#endif
