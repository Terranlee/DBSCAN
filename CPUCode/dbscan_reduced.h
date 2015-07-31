#ifndef __DBSCAN_REDUCED_H__
#define __DBSCAN_REDUCED_H__

#include "dbscan_grid.h"
#include "dbscan_reduced.h"

namespace clustering{
    class DBSCAN_Reduced : public DBSCAN_Grid{
    public:
        DBSCAN_Reduced(float eps, size_t min_elems);
        virtual ~DBSCAN_Reduced();

        virtual void fit();
        virtual void test();

    protected:
        /*****************************************************************************************/
        // Variables and functions for reduced precision implementation
        // The number of points in each cell is fixed, making the length of iteration to be fixed
        // The dataflow version of DBSCAN is based on this method, because hardware only support fixed length loop
        // The precision of the following implementation is controlled by m_max_num_point
        // Implemented in dbscan_reduced.cpp
        unsigned int m_max_num_point;

        void process_vector(std::vector<int>& vec);
        void reduce_precision(unsigned int max_num_point);

        // same function as the grid based algorithm
        // only add a restriction about the number of points in each cells
        bool search_in_neighbour_reduced(int point_id, HashType cell_key);
        void merge_in_neighbour_reduced(int point_id, HashType cell_key);
        int find_nearest_in_neighbour_reduced(int point_id, HashType cell_key);

        // the following three functions are new defined for this algorithm
        void determine_core_point_grid_reduced();
        void merge_clusters_reduced();
        void determine_boarder_point_reduced();

        // some test function
        void detect_cell_size();

    };
}

#endif

