#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>


using namespace std;

#include "union_find_utility_fun.hpp"
#include "graph_utility_fun.hpp"
#include "pdg_utility.hpp"




void printset(int* id, int N)
{
    for (int i=0; i<N; ++i){
        printf("%d ", id[i]);
    }
    printf("\n");
}


void print_cc_table(map<int, vector<int> > table){
    printf("print cc table: \n");
    map<int, vector<int> >::iterator itr;
    vector<int>::iterator itr_v;

    for (itr = table.begin(); itr != table.end(); ++itr){
        int root = itr -> first;
        vector<int> cousins = itr -> second;
        printf("%d ->", root);
        for (itr_v = cousins.begin(); itr_v != cousins.end(); ++itr_v){
            printf(" -> %d", *itr_v);
        }
        printf("\n");
    }
}


void print_vector(vector<int> a, char* name)
{
    printf("%s vector :", name);
    int n = a.size();
    for (int i =0; i< n; ++i){
        printf("%d (%d) ->", i, a[i]);
    }
    printf("\n");
}

void print_list(AdjListNode* L)
{
    printf("print list :");
    while(L){
        printf("%d -> ", L-> key);
        L = L -> next;
    }
    printf("\n");
}


/* main function */
int main()
{

    // read from data and output a graph with linkage table of edges */
    ifstream fin;
    string line;
    int N;

    fin.open("input_mapper_7.txt");
    if (!fin.is_open()) {
        cerr << "Problem appears when opening file!\n";
        exit(1);
    }

    // read the number of vertices
    fin>> N;
    getline(fin,line);

    float* inc_a = new float[N];
    float* inc_b = new float[N];
    int* v_id = new int[N];
    int* density = new int[N];

    // read vertices information
    for (int i=0; i<N; ++i){
        getline(fin, line);
        if (!line.length()) continue;
        sscanf(line.c_str(), "%d %f %f %d", v_id+i, inc_a+i, inc_b+i, density+i);
    }

    // add edges into the graph
    Graph* graph= createGraph(N);
//    Graph* graph_rel = createGraph(N);
//    Graph* tree = createGraph(N);

    int scr, dest;
    while (!fin.eof()){
        getline(fin,line);
        if (!line.length()) continue;
        sscanf(line.c_str(),"%d %d", &scr, &dest);
        add_edge(graph,dest,scr);
//        add_edge_upward(graph_rel, scr, dest);
//        add_edge_tree(tree, scr, dest);
    }

    fin.close();

    printGraph(graph);
//    printGraph(graph_rel);

    // print the tree
//    printf("\nThe following is a tree\n");
//    printGraph(tree);

    // delete an edge in the tree
//    scr = 0;
//    dest = 2;
//    delete_edge_in_tree(tree, scr, dest);
//
//    printf("\nThe following is a tree without edge (%d, %d)\n", scr, dest);
//    printGraph(tree);

//-------------------------------------------------------------//
    // define the filtration function f for vertices
    // and sort the vertices such that
    // f[0] <= f[1] <= ...<= f[N]
    float* filtration = new float[N];
    for (int i=0; i<N; ++i){
        filtration[i] = inc_a[i];
    }



//------------------------------------------------------------//
    WQUPC wqu(N);
    WQUPC wqu_ext(N);
//    printset(wqu.id, wqu.n_vertices);

    vector<Pdg_point> Pdg_ord_0 ;
    vector<Pdg_point> Pdg_ext_1;
    vector<Pairing> Pdg_ext_pairs;

    vector<int> cc_check(N,0);
    vector<int> cc_check_cousins(N,0);
    vector<int> UF_visited(N,0);
    vector<int> UF_id_color(N,-1);

    Graph* T = createGraph(N);

    for (int v = 0; v< N; ++v){

//        printf("\n v: %d \n", v);

        AdjListNode* neigh_u = graph -> linklist[v].head;


        if (neigh_u != NULL){

        //Part 1: search for the smallest ancestor and separate the cc of neighbors of v
            int root_u = wqu.root(neigh_u -> key);
            int oldest_ancestor_v = wqu.anc[root_u];
            int root_v = root_u;
            map<int, vector<int> > cc_table;

            // add the first neighbor u to the cc table with key "root_u"
            cc_check[root_u] = 1;
            cc_table[root_u] = vector<int> (1, neigh_u -> key);
            cc_check_cousins[neigh_u -> key] =1; // mark it as visited in the spanning tree for Ord_0;

            // pass to another neighbor of v;
            neigh_u = neigh_u -> next;
            while (neigh_u){
                cc_check_cousins[neigh_u -> key] = 1;
                root_u = wqu.root(neigh_u -> key);
                int oldest_ancestor_u = wqu.anc[root_u];

                // find the oldest ancestor among all neighbors of v
                if (oldest_ancestor_v > oldest_ancestor_u){
                    oldest_ancestor_v = oldest_ancestor_u;
                    root_v = root_u;
                }

                // update cc_table
                if (cc_check[root_u]==0){
                    // u belongs to a new cc
                    cc_check[root_u] = 1;
                    cc_table[root_u] = vector<int>(1,neigh_u-> key);
                }
                else{
                    cc_table[root_u].push_back(neigh_u -> key);
                    cc_check[root_u] += 1;
                }

                neigh_u = neigh_u -> next;
            } // end while(neigh_u)

//            print_cc_table(cc_table);
//            print_vector(cc_check_cousins, "cc_check_cousins");
//            print_vector(cc_check,"cc_check");

        //Part 2:  register the extended 1
            map<int, vector<int> >::iterator itr_cc;
            for (itr_cc = cc_table.begin(); itr_cc != cc_table.end(); ++itr_cc){

                int max_vertex_span;
                AdjListNode* L, *L_last, *L_head;

                root_u = itr_cc -> first;
                int u = itr_cc -> second[0];

//                printf("root: %d: \n", root_u);
//                printf("u: %d \n",u);

                // start searching the extended points within the cc of u.
                // step 1: search for the paths from u to its cousins w_1, w_2,....w_m
                int m = cc_check[root_u] -1 ;

                if (m > 0){
                    max_vertex_span = u;
                    L = newAdjListNode(u);
                    UF_visited[u] = 1;
                    UF_id_color[u] = u;
                    L_last = L;
                    L_head = L;
                }

                while (m > 0){
                    AdjListNode* w = T -> linklist[L->key].head;
                    while(w){
                        // if w is not visited, add to waiting list for spanning L
                        if (UF_visited[w->key] == 0){
                            // if w is one of the cousins of u
                            if (cc_check_cousins[w->key] ==1){
                                m = m-1;
                                UF_id_color[w -> key] = w -> key;
                            }

                            UF_visited[w->key]=1;
                            if (max_vertex_span< w->key)
                                max_vertex_span = w->key;

                            // add the new vertex to L
                            AdjListNode* newNode = newAdjListNode(w -> key);
                            L_last -> next = newNode;
                            L_last = newNode;
                        }

                        w = w -> next;
                    } // end while (w)

                    // move to another vertex in the waiting list
                    L = L -> next;
                }// end while(m>0)

//                print_vector(UF_visited, "UF_visited");

                // step 2: do Union Find on the tree T
                m = cc_check[root_u] -1;
                int r;
                if (m > 0) r = max_vertex_span;
                while (m>0){
//                    printf("r: %d, m: %d\n", r, m);
                    if (UF_visited[r] != 0){
                        AdjListNode* w = T -> linklist[r].head;
//                        while (w){
                        while (w && w -> key > r){
                        // by the construction of T, vertices > r are separate from vertices < r
//                            printf("w:");
//                            print_list(w);

//                            if (UF_visited[w -> key] != 0 && w->key > r ){
                            if (UF_visited[w-> key]!= 0){
                                int root_w = wqu_ext.root(w -> key);
                                int root_r = wqu_ext.root(r);

                                if (UF_id_color[root_r]== -1){
                                    UF_id_color[root_r] = UF_id_color[root_w];
                                    if (wqu_ext.sz[root_r]> wqu_ext.sz[root_w]){
                                        wqu_ext.id[root_w] = root_r;
                                        wqu_ext.sz[root_r] += wqu_ext.sz[root_w];
                                    }
                                    else{
                                        wqu_ext.id[root_r] = root_w;
                                        wqu_ext.sz[root_w] += wqu_ext.sz[root_r];
                                    } // end if
                                }
                                else if  (UF_id_color[root_w] == -1){
                                    if (wqu_ext.sz[root_r]> wqu_ext.sz[root_w]){
                                        // final root is "root_r"
                                        wqu_ext.id[root_w] = root_r;
                                        wqu_ext.sz[root_r] += wqu_ext.sz[root_w];
                                    }
                                    else{
                                        // final root is "root_w"
                                        wqu_ext.id[root_r] = root_w;
                                        wqu_ext.sz[root_w] += wqu_ext.sz[root_r];
                                        UF_id_color[root_w] = UF_id_color[root_r];
                                    } // end else if
                                }
                                else{
                                    // add new point to the persistence diagram
                                    Pdg_point newpoint_ext;
                                    newpoint_ext.x = v;
                                    newpoint_ext.y = r;
                                    Pdg_ext_1.push_back(newpoint_ext);

                                    Pairing newpoint;
                                    newpoint.pos_v = v;
                                    newpoint.neg_u = r;
                                    newpoint.neg_v = w -> key;

                                    // take the larger id color for the merged component
                                    if (wqu_ext.sz[root_r]> wqu_ext.sz[root_w]){
                                        // the final root of this cc is "root_r"
                                        if (UF_id_color[root_r]> UF_id_color[root_w]){
                                            newpoint.pos_u = UF_id_color[root_w];
                                            Pdg_ext_pairs.push_back(newpoint);
                                        }
                                        else{
                                            newpoint.pos_u = UF_id_color[root_r];
                                            Pdg_ext_pairs.push_back(newpoint);
                                            UF_id_color[root_r] = UF_id_color[root_w];
                                        }

                                        wqu_ext.id[root_w] = root_r;
                                        wqu_ext.sz[root_r] += wqu_ext.sz[root_w];
                                    }
                                    else{
                                        // the final root of this cc is "root_w"
                                        if (UF_id_color[root_r]> UF_id_color[root_w]){
                                            newpoint.pos_u = UF_id_color[root_w];
                                            Pdg_ext_pairs.push_back(newpoint);
                                            UF_id_color[root_w] = UF_id_color[root_r];
                                        }
                                        else{
                                            newpoint.pos_u = UF_id_color[root_r];
                                            Pdg_ext_pairs.push_back(newpoint);
                                        }

                                        wqu_ext.id[root_r] = root_w;
                                        wqu_ext.sz[root_w] += wqu_ext.sz[root_r];
                                    } // end if (wqu_ext.sz[root_r]> wqu_ext.sz[root_w])

                                    // erase the edge (r,w)
                                    delete_edge_in_tree(T, r, w->key);

                                    // pair a positive edge and a negative edge for pdg extended 1
                                    m = m-1;

                                } // end for the case (UF_id_color[root_r] != -1 && UF_id_color[root_w] != -1)
//                                print_vector(UF_id_color, "UF_id_color");
//                                printf("wqu_ext.id : ");
//                                for (int i =0; i< N; ++i) printf("%d -> ", wqu_ext.id[i]);
//                                printf("\n");
                            }// end if (UF_visited[w->key] !=0 && w->key > r->key)

                            // move to another neighbor of r in T
                            w = w -> next;

                        }// end while (w), finish merging the cc with vertex r

                    } // end if (UF_visited[r] != 0), skip the vertices that do not visited in spanning paths.

                    // move to a vertex in T lower than r
                    r = r-1;
                }// end while (m>0)
//*/
                // reinitialization
                if (cc_check[root_u] > 1){
                    while(L_head){
                        UF_visited[L_head -> key] = 0;
                        UF_id_color[L_head -> key] = -1;
                        wqu_ext.sz[L_head -> key] = 1;
                        wqu_ext.id[L_head -> key] = L_head -> key;

                        L_head = L_head -> next;
                    }
                }

            } // end for iterator map


            int oldest_ancestor_root = root_v;

        //Part 3: union all the different connected components of v
            for (itr_cc = cc_table.begin(); itr_cc != cc_table.end(); ++itr_cc){
                root_u = itr_cc -> first;
                vector<int> cousins_u = itr_cc -> second;

                // maintain the spanning tree for Ord 0
                for (int i=0; i< (int) cousins_u.size(); ++i){
                    cc_check_cousins[cousins_u[i]] = 0;
                    add_edge_tree(T, cousins_u[i], v);
                }

                // add (u,v) to Ord 0
                if (root_u != oldest_ancestor_root){
                    Pdg_point newpoint;
                    newpoint.x = wqu.anc[root_u];
                    newpoint.y = v;
                    Pdg_ord_0.push_back(newpoint);
                }// end if (root_u != oldest_ancestor_root)

                if (root_u != root_v){
                    if (wqu.sz[root_u] < wqu.sz[root_v]){
                        // the root after merging is "root_v"
                        wqu.sz[root_v] += wqu.sz[root_u];
                        wqu.id[root_u] = root_v;
                    }
                    else{
                        // the root after merging is "root_u"
                        wqu.sz[root_u] += wqu.sz[root_v];
                        wqu.id[root_v] = root_u;
                        root_v = root_u;
                    }
                } // end if (root_u != root_v)

                // free the root_u
                cc_check[root_u] = 0;
            } // end for loop itr_cc

            // attach v to the connect component
            wqu.id[v] = root_v;
            wqu.anc[root_v] = oldest_ancestor_v;
            wqu.sz[root_v] += 1;

//            printGraph(T);
        } // end if (neigh_u != NULL)

    } // end for (v =0; v< N; ++v)


    /* compute the relative persistence diagram */
    WQUPC wqu_rel(N);
    vector<Pdg_point> Pdg_rel_1;

    for (int v = N-1; v>=0; --v){
        AdjListNode* neigh_u = T -> linklist[v].head;
//        printf("v: %d \n ",v);
        if (neigh_u != NULL){
        //Part 1: search for all the roots of neighbors of v
            vector<int> root_neigh;
            int root_u;
            int largest_ancestor_v = -1;
            int largest_ancestor_u;
            int root_largest_ancestor_v;

            // by construction of T, vertices > v are totally separated from the ones <v
            while(neigh_u  && neigh_u -> key > v){
                root_u = wqu_rel.root(neigh_u -> key);
                largest_ancestor_u = wqu_rel.anc[root_u];

                if (largest_ancestor_v == -1 ||
                    largest_ancestor_v < largest_ancestor_u){
                    largest_ancestor_v = largest_ancestor_u;
                    root_largest_ancestor_v = root_u;
                }

                if (cc_check[root_u] == 0){
                    cc_check[root_u] = 1;
                    root_neigh.push_back(root_u);
                }
//                printf("u: %d ", neigh_u -> key);
//                print_vector(root_neigh,"root_neigh_u");

                neigh_u = neigh_u -> next;
            } // end while (neigh_u)

        //Part 2: union all the cc of neighbors of v
            if ((int) root_neigh.size() > 0){
                int root_v = root_largest_ancestor_v;
                vector<int>::iterator itr;
                for (itr = root_neigh.begin(); itr != root_neigh.end(); ++itr){
                    root_u = *itr;
                    if (root_u != root_largest_ancestor_v){
                        Pdg_point newpoint;
                        newpoint.x = wqu.anc[root_u];
                        newpoint.y = v;
                        Pdg_rel_1.push_back(newpoint);
                    }

                    if (root_u != root_v){
                        if (wqu_rel.sz[root_u] < wqu_rel.sz[root_v]){
                            // the root after merging is "root_v"
                            wqu_rel.sz[root_v] += wqu_rel.sz[root_u];
                            wqu_rel.id[root_u] = root_v;
                        }
                        else{
                            // the root after merging is "root_u"
                            wqu_rel.sz[root_u] += wqu_rel.sz[root_v];
                            wqu_rel.id[root_v] = root_u;
                            root_v = root_u;
                        }
                    } // end if (root_u != root_v)

                    cc_check[root_u] = 0;
                } // end for loop iterator vector

                // attache the vertex v to the merged cc
                wqu_rel.id[v] = root_v;
                wqu_rel.anc[root_v] = largest_ancestor_v;
                wqu_rel.sz[root_v] += 1;
            } // end if (root_neigh.size() >0)

        } // end if (neigh_u != NULL)
    }// end for v


    //-------------------------------------------------//


    printf("----------------------");
    printf("\n Ordinary dimension 0 \n");
    print_pdg(Pdg_ord_0, filtration);

    //------------------------------------------------//
    printf("---------------------");
    printf("\n Extended dimension 1 \n");
    print_pdg(Pdg_ext_1, filtration);

    printf("\n Extended dimension pairing \n");
    print_pdg_pair(Pdg_ext_pairs);

    //------------------------------------------------//
    printf("---------------------");
    printf("\n Relative dimension 1 \n");
    print_pdg(Pdg_rel_1, filtration);

    // -----------------------------------------------//
    printf("---------------------");
    printf("\n Spaning tree T");
    printGraph(T);


    return 0;
}
