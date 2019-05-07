#include "control_panel.h"

struct Graph* createGraph(int V, int E){
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	//Allocating space to structure graph

    graph->V = V;   //assigning values to structure elements that taken form user.

    graph->E = E;

    graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );
	//Creating "Edge" type structures inside "Graph" structure, the number of edge type structures are equal to number of edges

    return graph;
}

void BellmanFord(struct Graph* graph, char *source, char *dest){


}

void bellman_ford(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num){

	struct Graph* graph = createGraph(v_num, edge_num);    //calling the function to allocate space to these many vertices and edges
	//struct ListGraph *list_graph=createListGraph(v_num);
	//create_array();
	int j=0;
	for(int i=0; i<edge_num; i+=2){
		graph->edge[i].source=search(src[j], simple_hash(src[j]));
		graph->edge[i].destination=search(dest[j], simple_hash(dest[j]));
		graph->edge[i].weight=weights[j];
		graph->edge[i+1].source=search(dest[j], simple_hash(dest[j]));
                graph->edge[i+1].destination=search(src[j], simple_hash(src[j]));
                graph->edge[i+1].weight=weights[j];
		j++;

		//printf("src: %d, dest: %d, weight: %d\n", graph->edge[i].source, graph->edge[i].destination, graph->edge[i].weight);
	}
	int V = graph->V;
    	int E = graph->E;
	
	int dist[HASH_SIZE], predecessor[HASH_SIZE], weights_table[HASH_SIZE];

	for (int i=0;i<HASH_SIZE;i++){
		dist[i]=INT_MAX;
		predecessor[i]=0;
		weights_table[i]=0;
	}

    // This is initial step that we know , we initialize all distance to infinity except source.
	// We assign source distance as 0(zero)

    dist[search(port1, simple_hash(port1))] = 0;

    //The shortest path of graph that contain V vertices, never contain "V-1" edges. So we do here "V-1" relaxations
    for (int i = 1; i <= V-1; i++){
        for (int j = 0; j < E; j++){
            int u = graph->edge[j].source;

            int v = graph->edge[j].destination;

            int weight = graph->edge[j].weight;

            if (dist[u]!=INT_MAX && dist[u] + weight < dist[v]){
		    dist[v] = dist[u] + weight;
		    predecessor[v]=u;
		    weights_table[v]=weight;
	    }
	    //printf("SD: %s %s %d v: %d, u: %d\n", arr[v], arr[u], dist[v],v,u);
        }
    }

    // Actually upto now shortest path found. But BellmanFord checks for negative edge cycle. In this step we check for that
    // shortest distances if graph doesn't contain negative weight cycle.
	if (dist[search(port2, simple_hash(port2))]==INT_MAX){
                printf("Path does not exist.\n\n");
                return;
        }

    // If we get a shorter path, then there is a negative edge cycle.
    printf("Source\tDestination\tShortest Distance\n");

        printf("%s\t%s\t\t%d\n\n", port1, port2, dist[search(port2, simple_hash(port2))]);
        printf("Path\n");
	struct Edge edges[E];
	int dsn=search(port2, simple_hash(port2));

	for (int i=0; i<E;i++){
		edges[i].destination=dsn;
		edges[i].source=predecessor[dsn];
		edges[i].weight=weights_table[dsn];
		dsn=predecessor[dsn];
	}
	
	for (int i=E-1; i>=0; i--){
		if (edges[i].weight!=0)
			printf("%s\t%s\t\t%d\n", arr[edges[i].source], arr[edges[i].destination], edges[i].weight);
	}
	printf("\n");
	free(graph->edge);
	free(graph);

}
