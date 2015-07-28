#include <iostream>
#include <fstream>

#include "dbscan_lsh.h"

namespace clustering{
    DBSCAN_LSH::DBSCAN_LSH(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_LSH::DBSCAN_LSH(){}

    
    void DBSCAN_LSH::calculate_new_width(){
        // what is the cell width in the projection space?
        // we must make sure that not all clusters are merged in the projection space

    }

    void DBSCAN_LSH::rehash_data(){
        // use locality sensitive hashing to reassign the data to another grid
        for(unsigned int i=0; i<cl_d.size1(); i++){

        }
    }

    void DBSCAN_LSH::merge_in_projection(){

    }

    void DBSCAN_LSH::merge_clusters_lsh(){
        m_new_grid.resize(cl_d.size1());
        calculate_new_width();
        // TODO:
        // currently do the rehash and merge for a fixed number of iterations
        // later it should be updated to a heuristic version with a threshold to break
        for(int i=0; i<10; i++){
            rehash_data();
            merge_in_projection();
        }
    }

    void DBSCAN_LSH::fit(){
        prepare_labels(cl_d.size1());

        hash_construct_grid();
        determine_core_point_grid();

        // the merge clusters step is the most time consuming
        // try to improve the performance of this part
        float begin = get_clock();
        merge_clusters_lsh();
        float end = get_clock();
        cout<<end - begin<<endl;

        determine_boarder_point();
    }

    void DBSCAN_LSH::test(){
        // currently do nothing
        return;
    }
}
