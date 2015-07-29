#ifndef __DBSCAN_LSH_H__
#define __DBSCAN_LSH_H__

#include "dbscan_grid.h"

namespace clustering{
    class DBSCAN_LSH : public DBSCAN_Grid{
    public:
        typedef ublas::matrix<float> Functions;

        DBSCAN_LSH(float eps, size_t min_elems);
        virtual ~DBSCAN_LSH();

        virtual void fit();
        virtual void test();
    
    protected:
        /*****************************************************************************************/
        // Variables and functions for LSH DBSCAN method
        // The most time consuming step is merge clusters, especially in high dimension
        // We use locality sensitive hashing, not distance calculation, to do the merge
        // The algorithm is similar to that in dbscan_rehash.cpp, but that is only 2D with a fine grid
        // This is a approximate method, precision controlled by LSH dimension and the number of merge iteration
        // Implemented in dbscan_lsh.cpp
        
        // hash functions used by the LSH
        // din : the dimension of input data
        // dout : the dimension of output data
        // we hash the input data from din space to dout space
        Functions m_hash;

        // the output dimension of LSH
        // TODO:
        // currently we set the output dimension to be 6
        // later it should be update to another suitable number when we have enough experiments
        static constexpr unsigned int DOUT = 6;

        // two functions to generate the hash functions
        void hash_set_dimensions();
        void hash_generate();


        // a map between the index of point to the index of its cell in union find structure
        std::vector<int> m_point_to_uf;
        // a new grid to merge the original cells in the original grid
        std::vector<HashType> m_new_grid;

        // we have m_min_val in dbscan_grid.h
        // during the rehash, we have to set another new minimum value using this vector
        std::vector<float> m_new_min_val;
        float m_new_cell_width;

        // use locality sensitive hashing to rehash the data, and assign them to new grids
        // this can be accelerated by dataflow engine
        void rehash_data_projection();
        void merge_after_projection();
        void calculate_new_width();

        void merge_clusters_lsh();
    };
}

#endif
