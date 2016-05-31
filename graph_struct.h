#ifndef GRAPH_STRUCT_H_INCLUDED
#define GRAPH_STRUCT_H_INCLUDED

/* adjacency representation of graph */

struct AdjListNode
{
    int key;
    AdjListNode* next;
    AdjListNode* previous;
};

struct AdjList
{
    map<int, AdjListNode*> child;
    AdjListNode* head;
};


struct Graph
{
    int N;
    AdjList* linklist;
};

AdjListNode* newAdjListNode(int dest);
Graph* createGraph(int N);
void add_edge(Graph* graph, int src, int dest);
void add_edge_upward(Graph* graph, int src, int dest);
void printGraph(Graph* graph);
void test_adjgraph();


#endif // GRAPH_STRUCT_H_INCLUDED
