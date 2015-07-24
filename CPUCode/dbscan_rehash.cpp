#include <iostream>
#include <fstream>

#include "dbscan_rehash.h"

namespace clustering{
    DBSCAN_Rehash::DBSCAN_Rehash(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_Rehash::~DBSCAN_Rehash(){}

    void DBSCAN_Rehash::rehash_data_merge(){

    }

    void DBSCAN_Rehash::merge_clusters_rehash(){
        m_point_to_cell.resize(cl_d.size1());
        
    }

    void DBSCAN_Rehash::fit(){
        prepare_labels(cl_d.size1());

        // the following two steps are the same 
        // as the grid based algorithm
        hash_construct_grid();
        determine_core_point_grid();


    }

    void DBSCAN_Rehash::test(){
        // currently do nothing 
        return;
    }
}
