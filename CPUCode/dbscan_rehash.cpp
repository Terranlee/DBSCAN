#include <iostream>
#include <fstream>

#include "dbscan_rehash.h"

namespace clustering{
    DBSCAN_Rehash::DBSCAN_Rehash(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_Rehash::~DBSCAN_Rehash(){}

    void DBSCAN_Rehash::rehash_data_merge(){
        // move the grid m_cell_width / 2 to get a new grid
        std::unordered_map<int, int> new_grid;

        float min_x = m_min_x - m_cell_width / 2;
        float min_y = m_min_y - m_cell_width / 2;
        for(unsigned int i=0; i<cl_d.size1(); i++){
            if(!m_is_core[i])
                continue;

            int dx = int((cl_d(i,0) - min_x) / m_cell_width) + 1;
            int dy = int((cl_d(i,1) - min_y) / m_cell_width) + 1;
            int key = dx * (m_n_cols + 1) + dy;

            std::unordered_map<int, int>::iterator got = new_grid.find(key);
            if(got == new_grid.end()){
                new_grid.insert(std::make_pair(key, m_point_to_uf[i]));
            }
            else{
                int belong_id = got->second;
                int center_id = m_point_to_uf[i];
                uf.make_union(belong_id, center_id);
            }
        }
    }

    void DBSCAN_Rehash::merge_in_neighbour_rehash(int point_id, int center_id){
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        int cell_index = m_hash_grid.find(center_id)->second.ufID;
        // iterate on core points only
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, Cell>::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                // the difference between grid based algorithm and rehash algorithm
                // many neighbour cells may be merged already at the rehash function
                int belong_index = got->second.ufID;
                if(uf.find(cell_index) != uf.find(belong_index)){
                    for(unsigned int j=0; j<got->second.data.size(); j++){
                        int which = got->second.data.at(j);
                        if(!m_is_core[which])
                            continue;

                        float dist_sqr = 0.0;
                        for(unsigned int k=0; k<cl_d.size2(); k++){
                            float diff = cl_d(which, k) - cl_d(point_id, k);
                            dist_sqr += diff * diff;
                        }
                        if(dist_sqr < m_eps_sqr){
                            uf.make_union(belong_index, cell_index);
                            break;
                        }
                    }
                } //endof if(uf.find() != uf.find())
            }

            switch(i){
                case 2:
                    cell_iter = center_id - (m_n_cols + 1) - 2;
                    break;
                case 7:
                    cell_iter = center_id - 2;
                    break;
                case 12:
                    cell_iter = center_id + (m_n_cols + 1) - 2;
                    break;
                case 17:
                    cell_iter = center_id + (m_n_cols + 1) * 2 - 1;
                    break;
                default:
                    cell_iter = cell_iter + 1;
            }        
        }
    }

    void DBSCAN_Rehash::merge_clusters_rehash(){
        m_point_to_uf.resize(cl_d.size1());
        uf.init(m_hash_grid.size());

        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int ufid = iter->second.ufID;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int which = iter->second.data[i];
                m_point_to_uf[which] = ufid;
            }
        }

        rehash_data_merge();

        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int cell_id = iter->first;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int point_id = iter->second.data[i];
                if(!m_is_core[point_id])
                    continue;

                merge_in_neighbour_rehash(point_id, cell_id);
            }
        }

        cell_label_to_point_label();
    }

    void DBSCAN_Rehash::fit(){
        prepare_labels(cl_d.size1());

        // the following two steps are the same 
        // as the grid based algorithm
        hash_construct_grid();
        determine_core_point_grid();

        merge_clusters_rehash();

        determine_boarder_point();
    }

    void DBSCAN_Rehash::test(){
        // currently do nothing 
        return;
    }
}
