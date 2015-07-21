#ifndef __DBSCAN_REDUCED_H__
#define __DBSCAN_REDUCED_H__

#include "dbscan_grid.h"
#include "dbscan_reduced.h"

namespace clustering{
    class DBSCAN_Reduced : public DBSCAN_Grid{
    public:
        DBSCAN_Reduced(double eps, size_t min_elems);
        ~DBSCAN_Reduced();

        virtual void fit();
        virtual void test();

    private:
        /*****************************************************************************************/
        // variables and functions for reduced precision implementation
        // the number of points in each cell is fixed, making the length of iteration to be fixed
        // the precision of the following implementation will be reduced
        // implemented in dbscan_reduced.cpp
        unsigned int m_max_num_point;

        void process_vector(std::vector<int>& vec);
        void reduce_precision(unsigned int max_num_point);

        // same function as the grid based algorithm
        // only add a restriction about the number of points in each cells
        bool search_in_neighbour_reduced(int point_id, int cell_id);
        void merge_in_neighbour_reduced(int point_id, int cell_id, const std::unordered_map<int, int>& reverse);
        int find_nearest_in_neighbour_reduced(int point_id, int cell_id);

        void determine_core_point_grid_reduced();
        void merge_clusters_reduced();
        void determine_boarder_point_reduced();

        // some test function
        void detect_cell_size();

    };
}

#endif

