#ifndef UNION_FIND_UTILITY_FUN_HPP_INCLUDED
#define UNION_FIND_UTILITY_FUN_HPP_INCLUDED


#include "union_find_struct.h"

/* union find methods */
WQUPC::WQUPC()
{
    n_vertices = 10;
    id = new int[n_vertices];
    sz = new int[n_vertices];
    anc = new int[n_vertices];
    for (int i =0; i<n_vertices; ++i){
        sz[i]= 1;
        id[i]= i;
        anc[i] = i;
    }
}

WQUPC::WQUPC(int N)
{
    n_vertices = N;
    id = new int[N];
    sz = new int[N];
    anc = new int [N];
    for (int i = 0; i< N; ++i){
        sz[i] = 1;
        id[i] = i;
        anc[i] = i;
    }
}

int WQUPC::root(int i)
{
    // if i = id[i], then i is the root
    // according to the construction order, i is the smallest index of vertices in this subtree
    // thus, i represent the oldest element of the subtree
    while (i!=id[i]){
        id[i]= id[id[i]];
        i = id[i];
    }
    return i;
}

int WQUPC::compare(int p, int q)
{
    return root(p) == root(q);
}

void WQUPC::unite(int p, int q)
{
    int i = root(p);
    int j = root(q);
    if (sz[i]<= sz[j]){
        id[i]= j;
        sz[j]+= sz[i];
    }
    else{
        id[j] = i;
        sz[i] += sz[j];
    }
}



#endif // UNION_FIND_UTILITY_FUN_HPP_INCLUDED
