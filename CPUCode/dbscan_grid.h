#ifndef __DBSCAN_GRID_H__
#define __DBSCAN_GRID_H__

#include "dbscan.h"

namespace clustering{
    class DBSCAN_Grid : public DBSCAN{
    public:
        DBSCAN_Grid(float eps, size_t min_elems);
        virtual ~DBSCAN_Grid();
        
        virtual void fit();
        virtual void test();

    protected:
        /*****************************************************************************************/
        // variables and functions for grid DBSCAN method
        // currently for 2D only
        // implemented in dbscan_grid.cpp
        float m_cell_width;
        int m_n_rows;
        int m_n_cols;
        float m_min_x;
        float m_min_y;
        std::vector<bool> m_is_core;
        std::unordered_map<int, std::vector<int> > m_hash_grid;
        UnionFind uf;

        void grid_init(const int features_num);
        void getMinMax_grid(float* min_x, float* min_y, float* max_x, float* max_y );
        void cell_label_to_point_label(const std::unordered_map<int, int>& reverse_find);

        // check in neighbour function, only check the adjacent cells which is possible
        bool search_in_neighbour(int point_id, int cell_id);
        void merge_in_neighbour(int point_id, int cell_id, const std::unordered_map<int, int>& reverse);
        int find_nearest_in_neighbour(int point_id, int cell_id);
    
        // testing function
        void print_grid_info() const;
        void print_point_info() const;
        
        // four main steps for grid based DBSCAN clustering
        void hash_construct_grid();
        void determine_core_point_grid();
        void merge_clusters();
        void determine_boarder_point();

    };
}

#endif
