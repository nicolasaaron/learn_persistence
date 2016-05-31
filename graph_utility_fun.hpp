#ifndef GRAPH_UTILITY_FUN_HPP_INCLUDED
#define GRAPH_UTILITY_FUN_HPP_INCLUDED


#include "graph_struct.h"


/* init graph with adjacent linkage output */

// A utility function to create a new adjacency list node
AdjListNode* newAdjListNode(int dest)
{
    AdjListNode* newNode = new AdjListNode;
    newNode -> key = dest;
    newNode -> next = NULL;
    newNode -> previous = NULL;
    return newNode;
}


// A utility function taht creates a graph of N vertices
Graph* createGraph(int N)
{
    Graph* graph = new Graph;
    graph -> N = N;
    graph -> linklist = new AdjList[N];
    for (int i=0; i< N; ++i){
        graph -> linklist[i].head = NULL;
    }

    return graph;
}


// add an edge to an undirected graph
// edges in the adjacency list are (head, key) with head > key
void add_edge(Graph* graph, int src, int dest)
{
    if (src < dest){
        int temp = src;
        src = dest;
        dest = temp;
    }

    AdjListNode* newNode = newAdjListNode(dest);
    newNode -> next = graph -> linklist[src].head;
    graph -> linklist[src].head = newNode;
}

// edges in the adjacency list are (head, key) with head < key
void add_edge_upward(Graph* graph, int src, int dest)
{
    if (src > dest){
        int temp = src;
        src = dest;
        dest = temp;
    }

    AdjListNode* newNode = newAdjListNode(dest);
    newNode -> next = graph -> linklist[src].head;
    graph -> linklist[src].head = newNode;
}


// Add edges for a Tree structure
void add_edge_tree(Graph* tree, int src, int dest)
{
    // edge from "src" to "dest"
    AdjListNode* newNode = newAdjListNode(dest);
    newNode -> next = tree-> linklist[src].head;
    if (tree -> linklist[src].head == NULL)
        tree -> linklist[src].head = newNode;
    else{
        tree -> linklist[src].head -> previous = newNode;
        tree -> linklist[src].head = newNode;
    }
    tree -> linklist[src].child[dest] = newNode;

    // edge from "dest" to "src"
    AdjListNode* newNode2 = newAdjListNode(src);
    newNode2 -> next = tree -> linklist[dest].head;
    if (tree -> linklist[dest].head == NULL)
        tree -> linklist[dest].head = newNode2;
    else{
        tree -> linklist[dest].head -> previous = newNode2;
        tree -> linklist[dest].head = newNode2;
    }

    tree -> linklist[dest].child[src] = newNode2;
}


// delete an edge in a tree
// the edge is from "src" to "dest"
void delete_edge_in_tree(Graph* tree, int src, int dest)
{
    // delete edge from "src" to "dest"
    AdjListNode* node = tree -> linklist[src].child[dest];
    if (node -> previous == NULL && node -> next == NULL)
        tree -> linklist[src].head = NULL;
    else{
        if (node -> previous == NULL)
            tree -> linklist[src].head = node -> next;
        else
            node -> previous -> next = node -> next;

        if (node -> next == NULL)
            node -> previous -> next = NULL;
        else
            node -> next -> previous = node -> previous;
    }
    tree -> linklist[src].child.erase(dest);

    // delete edge from "dest" to "src"
    AdjListNode* node2 = tree -> linklist[dest].child[src];
    if (node2 -> previous == NULL && node2 -> next == NULL)
        tree -> linklist[dest].head = NULL;
    else{
        if (node2 -> previous == NULL)
            tree -> linklist[dest].head = node2 -> next;
        else
            node2 -> previous -> next = node2 -> next;

        if (node2 -> next == NULL)
            node2 -> previous -> next = NULL;
        else
            node2 -> next -> previous = node2 -> previous;
    }
    tree -> linklist[dest].child.erase(src);

    delete node;
    delete node2;
}

// A utility function to print the adjacency list representation of graph
void printGraph(Graph* graph)
{
    printf("\nprint graph \n");
    for (int v=0; v< graph -> N; ++v){
        AdjListNode* pCrawl = graph -> linklist[v].head;
//        printf("\n Adjacency list of vertex %d\n head", v);
        printf("%d ->", v);
        while (pCrawl){
            printf("-> %d", pCrawl->key);
            pCrawl = pCrawl -> next;
        }
        printf("\n");
    }
}

// test adjacency list representation of graph
void test_adjgraph()
{
    int N = 5;
    Graph* graph = createGraph(N);
    add_edge(graph, 0,1);
    add_edge(graph, 0,4);
    add_edge(graph, 1,2);
    add_edge(graph, 1,3);
    add_edge(graph, 1,4);
    add_edge(graph, 2,3);
    add_edge(graph, 3,4);

    printGraph(graph);
}


#endif // GRAPH_UTILITY_FUN_HPP_INCLUDED
