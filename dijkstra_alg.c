#include "control_panel.h"

struct AdjListNode* newAdjListNode(int dest, int weight){
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    
    return newNode;
}

// graph of V vertices
struct ListGraph* createListGraph(int V){
    struct ListGraph* graph = (struct ListGraph*) malloc(sizeof(struct ListGraph));
    
    graph->V = V;
    // array of adjacency lists
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

     // Initialize each adjacency list as empty
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to undirected graph
void addEdge(struct ListGraph* graph, int src, int dest, int weight){
    // Add an edge from src to dest
    // node added at the begining

    struct AdjListNode* node = graph->array[src].head;
    while (node != NULL) {
	if (node->dest != dest)
		node = node->next;
	else
		return;
    }
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    
    newNode->next = graph->array[src].head;
    
    graph->array[src].head = newNode;
    node = graph->array[dest].head;	
    
    while (node != NULL) {
        if (node->dest != src)
                node = node->next;
        else
                return;
    }

    // add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}
// min heap node
struct MinHeapNode* newMinHeapNode(int v, int dist){
    
    struct MinHeapNode* minHeapNode =(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// Min Heap
struct MinHeap* createMinHeap(int capacity){
    
    struct MinHeap* minHeap =(struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =(struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// swap two nodes of min heap
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// heapify at given idx
// updates position of nodes when they are swapped.
void minHeapify(struct MinHeap* minHeap, int idx){
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;

    if (smallest != idx)
    {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap){
    return minHeap->size == 0;
}

// extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    if (isEmpty(minHeap))
        return NULL;

    // store root node
    struct MinHeapNode* root = minHeap->array[0];

    // replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// decrease dist value of a given vertex v
void decreaseKey(struct MinHeap* minHeap, int v, int dist){
    // get index of v in  heap array
    int i = minHeap->pos[v];

    // get node and update dist value
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // swap node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// check if vertex in min heap
bool isInMinHeap(struct MinHeap *minHeap, int v){
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

// calulates distances of shortest paths from src to all vertices

void dijkstra(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num){
	int V=v_num;
	int dist[V];      // dist values used to pick minimum weight edge in cut
	int predecessor[V];
	int weights_table[V];
	for (int i=0; i<V; i++){
		predecessor[i]=-1;
		weights_table[i]=-1;
	}
	
	struct MinHeap* minHeap = createMinHeap(V);
	struct ListGraph* graph = createListGraph(V);
	
	for(int i=0; i<edge_num; i++)
		addEdge(graph, arr[simple_hash(src[i])], arr[simple_hash(dest[i])], weights[i]);
	
	// Initialize min heap with all vertices. dist value of all vertices
	for (int v = 0; v < V; ++v){
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}
	//set dist value of src vertex 0
	dist[arr[simple_hash(port1)]] = 0;
	decreaseKey(minHeap, arr[simple_hash(port1)], dist[arr[simple_hash(port1)]]);

	    // size of min heap = V
	   minHeap->size = V;

	    while (!isEmpty(minHeap)){
		// extract vertex with minimum distance value
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v; // store extracted vertex
		free(minHeapNode);
		// traverse through adjacent vertices of u and update distance values
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL){
		    int v = pCrawl->dest;

		    // If shortest distance to v is not finalized and distance to v ~ u < its previously calculated distance
		    if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v]){
			dist[v] = dist[u] + pCrawl->weight;
			predecessor[v]=u;
			weights_table[v]=pCrawl->weight;
			// update distance value in min heap
			decreaseKey(minHeap, v, dist[v]);
		    }
		    pCrawl = pCrawl->next;
		}
	    }
	struct AdjListNode* curr;
	for (int i=V-1; i>=0; i--){
		struct AdjListNode* pCrawl = graph->array[i].head;
		while (pCrawl != NULL){
			curr=pCrawl;
			pCrawl = pCrawl->next;
			free(curr);
		}
	}
	free(graph->array);
	free(graph);
	
	free(minHeap->array);
	free(minHeap->pos);
	free(minHeap);
		
	/*printf("Dijkstra:\nSource\tDestination\tShortest Distance\n"); 
        printf("%s\t%s\t\t%d\n\n", port1, port2, dist[arr[simple_hash(port2)]]);
	printf("Path\n");*/

	int done=0, E=edge_num;

        struct Edge edges2[E];
        int dsn=arr[simple_hash(port2)];

	for (int i=0; i<E; i++)
		edges2[i].weight=0;

        for (int i=0; i<E;i++){
		if (predecessor[dsn]==-1)
			break;
                if (!done){
			edges2[i].destination=dsn;
                	edges2[i].source=predecessor[dsn];
                        edges2[i].weight=weights_table[dsn];
                        if (edges2[i].source==arr[simple_hash(port1)])
                                done=1;
                } else
                        edges2[i].weight=0;
                dsn=predecessor[dsn];
	}
        for (int i=E-1; i>=0; i--){
                if (edges2[i].weight!=0)
                        continue;
			//printf("%s\t%s\t\t%d\n", map[edges2[i].source], map[edges2[i].destination], edges2[i].weight);
        }
	printf("\n");
}




