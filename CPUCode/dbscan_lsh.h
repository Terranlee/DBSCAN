#ifndef __DBSCAN_LSH_H__
#define __DBSCAN_LSH_H__

#include "dbscan_reduced.h"

namespace clustering{
    class DBSCAN_LSH : public DBSCAN_Reduced{
    public:

        DBSCAN_LSH(float eps, size_t min_elems, int num_iter);
        virtual ~DBSCAN_LSH();

        virtual void fit();
        virtual void test();
		
		// use this public interface to get the same result as the CUHK program, easy for comparison
		void output_same_result_cuhk();

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
        typedef ublas::matrix<float> Functions;

    protected:
        /*****************************************************************************************/
        // VARIABLES AND FUNCTIONS RELATED TO INITIALIZATION
        // the output dimension of LSH
        static const unsigned int DOUT;
        // after each locality sensitive hashing
        // we construct REDUNDANT numbers of new grids using different center point
        // and use these new grids to merge the small clusters
        static const unsigned int REDUNDANT;
        int m_num_iter;

        // set the new cell width in higher dimension
        void calculate_new_width();
        // two functions to generate the hash functions
        void hash_set_dimensions();



        /************************************************************************/
        // DATA STRUCTURE TO SUPPORT THIS HASH ALGORITHM
        // data structure to support reduced precision method in locality sensitive hashing
        // limit the number of points in each cell
        // and this is the number of points that is involved in calculation
        unsigned int m_total_num;
        // the width of the cell in the new space
        float m_new_cell_width;

        // a map between the index of point to the index of its cell in union find structure
        // use this together with union find data structure
        std::vector<int> m_point_to_uf;
        // a map between the index of non-core point and a integer
        // add this because the determine_core_point() will now repeat many times
        std::vector<int> m_core_map;
        // this two vectors to support the reduced precision algorithm
        std::vector<int> m_reduced_to_origin;
        std::vector<bool> m_origin_to_reduced;
        // map between point id and nearest distance
        // use this during determine_boarder_point
        std::unordered_map<int, float> m_boarder_dist;

        // FUNCTIONS TO SUPPORT THIS HASH ALGORITHM
        // init the data structures needed for this hash and merge
        void init_data_structure();
        int set_core_map();
        void main_iteration();
        void permute(std::vector<int>& intvec);



        /************************************************************************/
        // THE MOST IMPORTANT DATA STRUCTURE
        // hash functions used by the LSH
        // din : the dimension of input data
        // dout : the dimension of output data
        // we hash the input data from din space to dout space
        Functions m_hash;
        // use this to store the hash result
        std::vector<NewGrid> m_new_grid;
        // we have m_min_val in dbscan_grid.h
        // during the rehash, we have to set another new minimum value using this vector
        std::vector<NewCenter> m_new_min_val;
        // the merge result of one hashing method
        std::vector<MergeMap> m_merge_map;

        // THREE MOST IMPORTANT FUNCTIONS IN LSH_DBSCAN
        // generate new hash functions 
        void hash_generate();
        // use locality sensitive hashing to rehash the data, and assign them to new grids
        void rehash_data_projection();
        // construct the m_merge_map using the result of hashing
        // merge the points in the same cell
        void merge_cell_after_hash();


        

        /************************************************************************/
        // THE GRID BASED DBSCAN ALGORITHM ROUTINE
        // three sub functions for the lsh based method, may be called by the functions like *_lsh
        int determine_core_point_lsh_sub(CoreDetermine& cd);
        int merge_clusters_lsh_sub();
        void determine_boarder_point_lsh_sub();

        // reduced precision method in locality sensitive hashing
        void reduced_precision_lsh(unsigned int max_num_point);
        // all three steps of the grid based algorithm is now done by hashing
        void determine_core_point_lsh();
        void merge_clusters_lsh();
        void determine_boarder_point_lsh();
    };

}

#endif
