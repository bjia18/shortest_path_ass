#include "control_panel.h"

struct AdjListNode* newAdjListNode(int dest, int weight){
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct ListGraph* createListGraph(int V){
    struct ListGraph* graph = (struct ListGraph*) malloc(sizeof(struct ListGraph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct ListGraph* graph, int src, int dest, int weight){
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist){
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity){
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
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
        // The nodes to be swapped in min heap
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

// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap){
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist){
    // Get the index of v in  heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v){
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function

void dijkstra(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num){
	int V=v_num;
	int dist[V];      // dist values used to pick minimum weight edge in cut
	int predecessor[V];
	int weights_table[V];

	struct MinHeap* minHeap = createMinHeap(V);
	struct ListGraph* graph = createListGraph(V);
	
	for(int i=0; i<edge_num; i++){
		addEdge(graph, arr[simple_hash(src[i])], arr[simple_hash(dest[i])], weights[i]);
		addEdge(graph, arr[simple_hash(dest[i])], arr[simple_hash(src[i])], weights[i]);
                //printf("src: %d, dest: %d, weight: %d\n", graph->edge[i].source, graph->edge[i].destination, graph->edge[i].weight);
        }
		    // Initialize min heap with all vertices. dist value of all vertices
	for (int v = 0; v < V; ++v){
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}

	    // Make dist value of src vertex as 0 so that it is extracted first
	minHeap->array[arr[simple_hash(port1)]] = newMinHeapNode(arr[simple_hash(port1)], dist[arr[simple_hash(port1)]]);
	minHeap->pos[arr[simple_hash(port1)]] = arr[simple_hash(port1)];
	dist[arr[simple_hash(port1)]] = 0;
	decreaseKey(minHeap, arr[simple_hash(port1)], dist[arr[simple_hash(port1)]]);

	    // Initially size of min heap is equal to V
	   minHeap->size = V;

	    // In the followin loop, min heap contains all nodes
	    // whose shortest distance is not yet finalized.
	    while (!isEmpty(minHeap)){
		// Extract the vertex with minimum distance value
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v; // Store the extracted vertex number

		// Traverse through all adjacent vertices of u (the extracted
		// vertex) and update their distance values
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL){
		    int v = pCrawl->dest;

		    // If shortest distance to v is not finalized yet, and distance to v
		    // through u is less than its previously calculated distance
		    if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v]){
			dist[v] = dist[u] + pCrawl->weight;
			predecessor[v]=u;
			weights_table[v]=pCrawl->weight;
			// update distance value in min heap also
			decreaseKey(minHeap, v, dist[v]);
		    }
		    pCrawl = pCrawl->next;
		}
	    }
	//printf("Dijkstra:\nSource\tDestination\tShortest Distance\n"); 
        //printf("%s\t%s\t\t%d\n\n", port1, port2, dist[arr[simple_hash(port2)]]);
	//printf("Path\n");
	int E=edge_num;
        struct Edge edges[E];
        int dsn=arr[simple_hash(port2)];

        int done=0;
        for (int i=0; i<E;i++){
                edges[i].destination=dsn;
                edges[i].source=predecessor[dsn];
                if (!done){
                        edges[i].weight=weights_table[dsn];
                        if (edges[i].source==arr[simple_hash(port1)])
                                done=1;
                } else
                        edges[i].weight=0;
                dsn=predecessor[dsn];
        }
        /*for (int i=E-1; i>=0; i--){
                if (edges[i].weight!=0)
                        printf("%s\t%s\t\t%d\n", map[edges[i].source], map[edges[i].destination], edges[i].weight);
        }
	printf("\n");*/


    // print the calculated shortest distances	

}




