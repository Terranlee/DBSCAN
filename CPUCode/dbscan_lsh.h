#ifndef __DBSCAN_LSH_H__
#define __DBSCAN_LSH_H__

#include "dbscan_reduced.h"

namespace clustering{
    class DBSCAN_LSH : public DBSCAN_Reduced{
    public:
        typedef ublas::matrix<float> Functions;

        DBSCAN_LSH(float eps, size_t min_elems);
        virtual ~DBSCAN_LSH();

        virtual void fit();
        virtual void test();
        
        template<typename T>
        class PairHash{
        public:
            size_t operator()(const std::pair<T, T>& p) const{
                return std::hash<T>()(p.first) ^ (std::hash<T>()(p.second));
            }
        };

        // DimType is the output hash result in high dimension(8 dimension)
        // use int128 to represent the hash result, 
        // so the data range in each dimension is [-32768, 32767],
        // should be enough for most cases
        typedef std::pair<int64_t, int64_t> DimType;
        typedef std::vector<DimType> NewGrid;
        typedef std::unordered_map<DimType, std::vector<int>, PairHash<int64_t> > MergeMap;

        // this is the center point we use to construct grid in high dimension
        typedef std::vector<float> NewCenter;

        // use this matrix to determine whether a point is core point or not
        typedef ublas::matrix<int> CoreDetermine;

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
        static const unsigned int DOUT;

        // set the new cell width in higher dimension
        void calculate_new_width();

        // two functions to generate the hash functions
        void hash_set_dimensions();
        void hash_generate();

        void permute(std::vector<int>& intvec);
        /************************************************************************/
        // DATA STRUCTURE TO SUPPORT THIS HASH ALGORITHM
        // a map between the index of point to the index of its cell in union find structure
        // use this together with union find data structure
        std::vector<int> m_point_to_uf;
        // a map between the index of non-core point and a integer
        // add this because the determine_core_point() will now repeat many times
        std::vector<int> m_core_map;

        // the width of the cell in the new space
        float m_new_cell_width;

        // data structure to support reduced precision method in locality sensitive hashing
        // limit the number of points in each cell
        // and this is the number of points that is involved in calculation
        unsigned int m_total_num;
        std::vector<int> m_reduced_to_origin;
        std::vector<bool> m_origin_to_reduced;  // the size is 

        std::unordered_map<int, float> m_boarder_dist;


        /************************************************************************/
        // THE MOST IMPORTANT DATA STRUCTURE
        // after each locality sensitive hashing
        // we construct REDUNDANT numbers of new grids using different center point
        // and use these new grids to merge the small clusters
        static const unsigned int REDUNDANT;
        std::vector<NewGrid> m_new_grid;
        // we have m_min_val in dbscan_grid.h
        // during the rehash, we have to set another new minimum value using this vector
        std::vector<NewCenter> m_new_min_val;
        // the merge result of one hashing method
        std::vector<MergeMap> m_merge_map;



        // init the data structures needed for this hash and merge
        void init_data_structure();
        int set_core_map();
        void main_iteration();
        
        // we do the following two functions again and again, and use the merged results to determine core points, or merge small clusters

        // use locality sensitive hashing to rehash the data, and assign them to new grids
        // this can be accelerated by dataflow engine
        void rehash_data_projection();
        // construct the m_merge_map using the result of hashing
        // merge the points in the same cell
        void merge_cell_after_hash(bool possible);

        // return the number of small clusters that are merged in this iteration
        // use the return value to terminate the program
        void determine_core_using_merge(int index);
        int merge_small_clusters();

        // reduced precision method in locality sensitive hashing
        void reduced_precision_lsh(unsigned int max_num_point);

        // all three steps of the grid based algorithm is now done by hashing
        void determine_core_point_lsh();
        void merge_clusters_lsh();
        void determine_boarder_point_lsh();
    };

}

#endif
