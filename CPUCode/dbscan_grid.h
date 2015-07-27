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
        // update the original algorithm to support multiple dimension
        // implemented in dbscan_grid.cpp
        float m_cell_width;

        std::vector<float> m_min_val;
        std::vector<int> m_n_cnt;
        
        std::vector<bool> m_is_core;
        std::unordered_map<HashType, Cell> m_hash_grid;
        UnionFind uf;

        void grid_init(const int features_num);
        void getMinMax_grid(std::vector<float>& min, std::vector<float>& max);
        void cell_label_to_point_label();

        // check in neighbour function, only check the adjacent cells which is possible
        bool search_in_neighbour(int point_id, int cell_id);
        void merge_in_neighbour(int point_id, int cell_id);
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
