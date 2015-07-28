#include <iostream>
#include <fstream>

#include "dbscan_rehash.h"

namespace clustering{
    DBSCAN_Rehash::DBSCAN_Rehash(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_Rehash::~DBSCAN_Rehash(){}

    void DBSCAN_Rehash::rehash_data_merge(){
        // move the grid m_cell_width / 2 to get a new grid
        std::unordered_map<HashType, int> new_grid;

        std::vector<float> min_val(m_min_val.size());
        for(unsigned int i=0; i<min_val.size(); i++)
            min_val[i] = m_min_val[i] - m_cell_width / 2;
        std::vector<int> delta(m_min_val.size());

        for(unsigned int i=0; i<cl_d.size1(); i++){
            if(!m_is_core[i])
                continue;
            for(unsigned int k=0; k<cl_d.size2(); k++)
                delta[k] = int((cl_d(i, k) - min_val[k]) / m_cell_width) + 1;
            HashType key = mi.hash(delta);

            std::unordered_map<HashType, int>::iterator got = new_grid.find(key);
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

    void DBSCAN_Rehash::merge_in_neighbour_rehash(int point_id, HashType center_key){
        mi.set_start(center_key);
        HashType cell_iter = mi.get();
        int num_of_neighbour = mi.get_counter();
        int cell_index = m_hash_grid.find(center_key)->second.ufID;

        // iterate on core points only
        for(int i=0; i<num_of_neighbour; i++){
            std::unordered_map<HashType, Cell>::const_iterator got = m_hash_grid.find(cell_iter);
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
            cell_iter = mi.next();
        }
    }

    void DBSCAN_Rehash::merge_clusters_rehash(){
        m_point_to_uf.resize(cl_d.size1());
        uf.init(m_hash_grid.size());

        for(std::unordered_map<HashType, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int ufid = iter->second.ufID;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int which = iter->second.data[i];
                m_point_to_uf[which] = ufid;
            }
        }

        rehash_data_merge();

        for(std::unordered_map<HashType, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            HashType center_key = iter->first;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int point_id = iter->second.data[i];
                if(!m_is_core[point_id])
                    continue;

                merge_in_neighbour_rehash(point_id, center_key);
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
