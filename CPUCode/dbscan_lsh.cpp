#include <iostream>
#include <fstream>

#include "dbscan_lsh.h"

namespace clustering{
    DBSCAN_LSH::DBSCAN_LSH(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_LSH::DBSCAN_LSH(){}

    void DBSCAN_LSH::rehash_data(){
        // use locality sensitive hashing to reassign the data to another grid
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
