#include <iostream>
#include <climits>
#include <algorithm>
#include <functional>

#include "dbscan_dfe.h"

namespace clustering{

    DBSCAN_DFE::DBSCAN_DFE(double eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_DFE::~DBSCAN_DFE(){
        delete[] input_data;
    }

    void set_reduced_precision(int r_num){
        reduced_num = r_num;
    }

    void merge_clusters_dfe(){

    }

    void prepare_data(){
        int length = reduced_num * m_n_rows * m_n_cols * 2;
        cout<<"total length: "<<length<<endl;
        is_valid.resize(length);
        input_data = new float[length];

        for(std::unordered_map<int, std::vector<int> >)
    }

    // virtual functions derived from DBSCAN_Grid
    void DBSCAN_DFE::fit(){
        prepare_labels(cl_d.size1());

        hash_construct_grid();
        determine_core_point();

        prepare_data();
        merge_clusters_dfe();
        determine_core_point();

    }

    void DBSCAN_DFE::test(){
        // currently do nothing
        return;
    }

}

