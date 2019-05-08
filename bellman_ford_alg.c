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

void bellman_ford(char src[][4], char dest[][4], int *weights, char *port1, char *port2, int edge_num, int v_num){

	struct Graph* graph = createGraph(v_num, edge_num);

	int V = graph->V;
        int E = graph->E;
    	//calling the function to allocate space to these many vertices and edges
	
	int j=0;
	for(int i=0; i<edge_num; i+=2){
		graph->edge[i].source=arr[simple_hash(src[j])];
		graph->edge[i].destination=arr[simple_hash(dest[j])];
		graph->edge[i].weight=weights[j];
		graph->edge[i+1].source=arr[simple_hash(dest[j])];
                graph->edge[i+1].destination=arr[simple_hash(src[j])];
                graph->edge[i+1].weight=weights[j];
		j++;

		//printf("src: %d, dest: %d, weight: %d\n", graph->edge[i].source, graph->edge[i].destination, graph->edge[i].weight);
	}
	
	int dist[E], predecessor[E], weights_table[E];

	for (int i=0;i<E;i++){
		dist[i]=INT_MAX;
		predecessor[i]=0;
		weights_table[i]=0;
	}

    // This is initial step that we know , we initialize all distance to infinity except source.
	// We assign source distance as 0(zero)

    dist[arr[simple_hash(port1)]] = 0;

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
	if (dist[arr[simple_hash(port2)]]==INT_MAX){
                printf("Path does not exist.\n\n");
                return;
        }

    // If we get a shorter path, then there is a negative edge cycle.
    //printf("Bellman-Ford:\nSource\tDestination\tShortest Distance\n");
	printf("Source\tDestination\tShortest Distance\n");
        printf("%s\t%s\t\t%d\n\n", port1, port2, dist[arr[simple_hash(port2)]]);
        printf("Path\n");
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
	for (int i=E-1; i>=0; i--){
		if (edges[i].weight!=0)
			printf("%s\t%s\t\t%d\n", map[edges[i].source], map[edges[i].destination], edges[i].weight);
	}
	printf("\n");
	free(graph->edge);
	free(graph);

}
