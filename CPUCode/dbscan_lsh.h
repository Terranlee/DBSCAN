#ifndef __DBSCAN_LSH_H__
#define __DBSCAN_LSH_H__

#include "dbscan_grid.h"

namespace clustering{
    class DBSCAN_LSH : public DBSCAN_Grid{
    public:
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
        
        // a map between the index of point to the index of its cell in union find structure
        std::vector<int> m_point_to_uf;
        // a new grid to merge the original cells in the original grid
        std::vector<HashType> m_new_grid;

        // the collection of all hash functions
        LSH m_hash;

        // use locality sensitive hashing to rehash the data, and assign them to new grids
        // this can be accelerated by dataflow engine
        void rehash_data();
        void merge_in_projection();
        void calculate_new_width();

        void merge_clusters_lsh();
    };
}

#endif
