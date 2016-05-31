#ifndef UNION_FIND_STRUCT_H_INCLUDED
#define UNION_FIND_STRUCT_H_INCLUDED

/* union find structure */
class WQUPC
{
public:
    WQUPC();
    WQUPC(int N);
    int root(int i); // find the oldest elements of the component of v_i
    int compare(int p, int q);  // compare whether v_p and v_q belong to the same component
    void unite(int p, int q); // union the two components to which v_p and v_q belong
public:
    int *id; // trees related to the union-find algorithm
    int *anc; // anc[i] the ancestor of the component id[i]=i;
    int *sz; // the size of subtrees
    int n_vertices;
};

#endif // UNION_FIND_STRUCT_H_INCLUDED
