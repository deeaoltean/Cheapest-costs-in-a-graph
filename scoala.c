/*
 * Project: Homework 3 - Data Structures
 * Name: OLTEAN ANDREEA-DIANA
 * Group: 314CD
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "Util.h"

/*
 * Name function: createGraphAdjList
 * Return: the adress of a new graph
 * Arguments: number of nodes in the graph
 * Purpose: allocate memory for a new graph
 */
TGraphL* createGraphAdjList(int numberOfNodes){
	TGraphL *list;
	list = (TGraphL*)malloc(sizeof(TGraphL));
	if(list == NULL) {
		fprintf(stderr, "Not enough memory");
		return NULL;
	}
	list->nn=numberOfNodes;
	list->adl=(ATNode*)malloc(numberOfNodes * sizeof(ATNode));
	if(list->adl == NULL) {
		fprintf(stderr, "Not enough memory");
		return NULL;
	} 

	//initialise each list
	int i;
	for(i = 0; i < list->nn; i++) {
		list->adl[i]=NULL;
	}

	return list;
}

/*
 * Name function: addEdgeList
 * Return: void (it does not return a value)
 * Arguments: the graph, two nodes and the cost of an edge
 * Purpose: add the new edge into the graph
 */
void addEdgeList(TGraphL* graph, int v1, int v2, int c_edge){
	TNode *node1, *node2;
	node1 = (TNode*)malloc(sizeof(TNode));
	if(node1 == NULL) {
		fprintf(stderr, "Not enough memory");
		return;
	}	

	//add to the list of the first node
	node1->c = c_edge;
	node1->v = v2;
	node1->next = graph->adl[v1];
	graph->adl[v1] = node1;

	node2 = (TNode*)malloc(sizeof(TNode));
	if(node2 == NULL) {
		fprintf(stderr, "Not enough memory");
		return;
	}

	//add to the list of the second node
	node2->c = c_edge;
	node2->v = v1;
	node2->next = graph->adl[v2];
	graph->adl[v2] = node2;
}

/*
 * Name function: bfsGL
 * Return: void (it does not return a value)
 * Arguments: the graph, the value of a node, an array of visited nodes
 * Purpose: breadth first search
 */
void bfsGL(TGraphL* graph, int s, int *visit){
	Queue *queue = createQueue();
	TNode *node;
	enqueue(queue, s);
	visit[s] = 1;
	for(int i = 0; i < graph->nn; i++) {
		for(node = graph->adl[front(queue)]; node != NULL; node = node->next){
			if(visit[node->v] == 0) {
				visit[node->v] = 1;
				enqueue(queue, node->v);
			}
		}
		dequeue(queue);
	}
	destroyQueue(queue);
}

/*
 * Name function: c_edgeCase
 * Return: void (it does not return a value)
 * Arguments: the graph, the number of conex elements, the cost of a node and
 * edge, the number of nodes and the file I am writing in
 * Purpose: calculate the minimum cost of a route
 */
void c_edgeCase(TGraphL *graph, int n_conexe, int c_node, int c_edge,
		int n_nodes, FILE *out){
	//create an array to determine the number of conex elements
	int *visit;
	visit = (int*) calloc(graph->nn, sizeof(int));
	for(int i = 0; i < graph->nn; i++){
		if(visit[i] == 0){
			n_conexe++;
			//for each not visited element, find the neighbours
			bfsGL(graph,i,visit);
		}
	}
	free(visit);
	fprintf(out, "%d\n", n_conexe * c_node + c_edge * (n_nodes - n_conexe));
}

/*
 * Name function: destroyGraphAdjList
 * Return: void (it does not return a value)
 * Arguments: the graph
 * Purpose: free all the memory
 */
void destroyGraphAdjList(TGraphL* graph){
	for(int i = 0; i < graph->nn; i++){
		for(TNode* it = graph->adl[i]; it != NULL; ){
		  TNode* p = it;
		  it = it->next;
			free(p);
		}
	}
	free(graph->adl);
	free(graph);
}

int main() {
	char input_filename[] = "scoala.in";
	char output_filename[] = "scoala.out";
	FILE *in = fopen(input_filename, "rt");
	FILE *out = fopen(output_filename, "w");

	int n_conexe = 0;
	if(in == NULL) {
		fprintf(stderr, "ERROR: Can't open file %s", input_filename);
		return -1;
	}
	if(out == NULL) {
		fprintf(stderr, "ERROR: Can't open file %s", output_filename);
		return -1;
	}

	//read the information from the file
	int k, n_nodes, n_edges, c_node, c_edge;
	fscanf(in, "%d", &k);

	int j;
	//read the information for the number of enteries
	for(j = 0; j < k; j++) {
		fscanf(in, "%d %d", &n_nodes, &n_edges);
		fscanf(in, "%d %d", &c_edge, &c_node);
		TGraphL *graph;
		graph = createGraphAdjList(n_nodes);
		int i;
		//add the edges into the graph
		for(i = 0; i < n_edges; i++) {
			int node1, node2;
			fscanf(in, "%d %d", &node1, &node2);
			addEdgeList(graph, node1 - 1, node2 - 1, c_edge);
		}
		if(c_node < c_edge) {
			fprintf(out, "%d\n", c_node * n_nodes);
		} else {
			c_edgeCase(graph, n_conexe, c_node, c_edge, n_nodes, out);
		}
		destroyGraphAdjList(graph);
	}
	fclose(in);
	fclose(out);
	return 0;
}
