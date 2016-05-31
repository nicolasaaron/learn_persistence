#ifndef PDG_UTILITY_HPP_INCLUDED
#define PDG_UTILITY_HPP_INCLUDED

#include "pdg_struct.h"

void print_pdg(vector<Pdg_point> pdg, float* f){

    printf("persistence diagram \n");
    vector<Pdg_point>::iterator pt;
    for (pt = pdg.begin(); pt!= pdg.end(); ++pt){
        printf("( %f, %f) with (%d %d)\n", f[(*pt).x], f[(*pt).y],(*pt).x, (*pt).y);
    }
}

void print_pdg_pair(vector<Pairing> pdg_pair)
{
    printf("positive <--> negative\n");
    vector<Pairing>::iterator pt;
    for (pt = pdg_pair.begin(); pt!= pdg_pair.end(); ++pt){
        printf("(%d, %d) <--> (%d, %d)\n", pt -> pos_u, pt -> pos_v, pt-> neg_u, pt -> neg_v);
    }
}

#endif // PDG_UTILITY_HPP_INCLUDED
