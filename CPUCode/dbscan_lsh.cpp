#include <iostream>
#include <cmath>
#include <fstream>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "dbscan_lsh.h"

namespace clustering{
    DBSCAN_LSH::DBSCAN_LSH(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_LSH::DBSCAN_LSH(){}

    void DBSCAN_LSH::hash_set_dimensions(){
        // the function matrix has dout lines, and din rows
        // the line of the matrix can be used to do projection
        m_hash = Functions(DOUT, cl_d.size2());
    }

    void DBSCAN_LSH::hash_generate(){
        // may be changed to srand((unsigned) time(NULL))
        srand(0);
        for(unsigned int i=0; i<m_hash.size1(); i++)
            for(unsigned int j=0; j<m_hash.size2(); j++)
                m_hash(i, j) = MULTIPLIER * float(rand()) / float(RAND_MAX);
    }

    void DBSCAN_LSH::calculate_new_width(){
        // what is the cell width in the projection space?
        // we must make sure that not all clusters are merged in the projection space

        //float sqr_dout = std::sqrt(float(dout));
        // the cell_width in high dimension is also eps theoratically
        // but consider the possibility of wrong classification, we multiply it by 0.5, and do more iteration
        float eps = std::sqrt(m_eps_sqr);
        m_new_cell_width = eps * 0.5;
    }

    void DBSCAN_LSH::rehash_data_projection(){
        // use locality sensitive hashing to reassign the data to another grid
        for(unsigned int i=0; i<DOUT; i++){
            float mini = 0.0f;
            for(unsigned int j=0; j<cl_d.size2(); j++)

            m_new_min_val[i] = 
        }
        for(unsigned int i=0; i<cl_d.size1(); i++){

        }
    }

    void DBSCAN_LSH::merge_after_projection(){

    }

    void DBSCAN_LSH::merge_clusters_lsh(){
        m_new_grid.resize(cl_d.size1());
        m_new_min_val.resize(DOUT);
        m_point_to_uf.resize(cl_d.size1());
        uf.init(cl_d.size1());

        for(std::unordered_map<HashType, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int ufid = iter->second.ufID;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int which = iter->second.data[i];
                m_point_to_uf[which] = ufid;
            }
        }

        calculate_new_width();
        hash_set_dimensions();

        // TODO:
        // currently do the rehash and merge for a fixed number of iterations
        // later it should be updated to a heuristic version with a threshold to break
        for(int i=0; i<10; i++){
            hash_generate();
            rehash_data_projection();
            merge_after_projection();
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
