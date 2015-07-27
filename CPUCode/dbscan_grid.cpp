#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>
#include <cassert>
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <vector>

#include "dbscan_grid.h"

namespace clustering{
    DBSCAN_Grid::DBSCAN_Grid(float eps, size_t min_elems) : DBSCAN(eps, min_elems){}
    DBSCAN_Grid::~DBSCAN_Grid(){}
    
    // the following are for grid base method
    void DBSCAN_Grid::grid_init(const int features_num){
        float sq = sqrt(float(features_num));
        float eps = sqrt(m_eps_sqr);
        m_cell_width = eps / sq;
    }

    void DBSCAN_Grid::getMinMax_grid(std::vector<float>& min_vec, std::vector<float>& max_vec){
        assert(min_vec.size() == cl_d.size2());
        assert(max_vec.size() == cl_d.size2());
        for(unsigned int i=0; i<cl_d.size1(); i++){
            for(unsigned int j=0; j<cl_d.size2(); j++){
                if(cl_d(i, j) > max_vec[j])
                    max_vec[j] = cl_d(i, j);
                if(cl_d(i, j) < min_vec[j])
                    min_vec[j] = cl_d(i, j);
            }
        }
    }

    void DBSCAN_Grid::hash_construct_grid(){
        unsigned int features_num = cl_d.size2();
        grid_init(features_num);

        std::vector<float> min_vec(features_num, std::numeric_limits<float>::max());
        std::vector<float> max_vec(features_num, std::numeric_limits<float>::min());
        getMinMax_grid(min_vec, max_vec);
        
        m_min_val.resize(features_num);
        std::copy(min_vec.begin(), min_vec.end(), m_min_val.begin());

        m_n_cnt.resize(features_num);
        for(unsigned int i=0; i<features_num; i++)
            m_n_cnt[i] = int((max_vec[i] - min_vec[i]) / m_cell_width) + 1;

        unsigned int length = cl_d.size1();
        int uf_counter = 0;
        for(int i=0; i<length; i++){
            int dx = int((cl_d(i,0) - min_x) / m_cell_width) + 1;
            int dy = int((cl_d(i,1) - min_y) / m_cell_width) + 1;
            int key = dx * (nCols + 1) + dy;

            std::unordered_map<HashType, Cell>::iterator got = m_hash_grid.find(key);
            if(got == m_hash_grid.end()){
                Cell c;
                c.ufID = uf_counter++;
                c.data.push_back(i);
                m_hash_grid.insert(std::make_pair(key,c));
            }
            else
                got->second.data.push_back(i);
        }
        m_n_rows = nRows;
        m_n_cols = nCols;
        //cout<<"n_rows:"<<m_n_rows<<" n_cols:"<<m_n_cols<<endl;
        //print_grid_info(cl_d);
    }

    bool DBSCAN_Grid::search_in_neighbour(int point_id, int center_id){
        // TODO: dimension related function
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;
        unsigned int counter = 0;
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, Cell>::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.data.size(); j++){
                    int which = got->second.data.at(j);

                    float dist_sqr = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        float diff = cl_d(which, k) - cl_d(point_id, k);
                        dist_sqr += diff * diff;
                    }

                    if(dist_sqr < m_eps_sqr)
                        counter++;
                    // here we use '>', because it should not include the center point itself
                    if(counter > m_min_elems)
                        return true;
                }
            }

            // these represent the search neighbour routine
            // the change of cell_iter is fixed in all _in_neighbour function
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
        return false;
    }

    void DBSCAN_Grid::determine_core_point_grid(){
        m_is_core.resize(cl_d.size1(), false);
        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            //  here we use '>', because it should not include the central point itself
            if(iter->second.data.size() > m_min_elems){
                for(unsigned int i=0; i<iter->second.data.size(); i++){
                    int which = iter->second.data.at(i);
                    m_is_core[which] = true;
                }
            }
            else{
                int cell_id = iter->first;
                for(unsigned int i=0; i<iter->second.data.size(); i++){
                    int point_id = iter->second.data.at(i);
                    bool result = search_in_neighbour(point_id, cell_id);
                    m_is_core[point_id] = result;
                }
            }
        }
        //print_point_info(cl_d);
    }

    void DBSCAN_Grid::merge_in_neighbour(int point_id, int center_id){
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        int cell_index = m_hash_grid.find(center_id)->second.ufID;
        // iterate on core points only
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, Cell>::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
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
                        int belong_index = got->second.ufID;
                        uf.make_union(belong_index, cell_index);
                        break;
                    }
                }
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

    void DBSCAN_Grid::cell_label_to_point_label(){
        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            // key is the index in the hash_grid, key_index is the corresponding index in the union_find structure
            int key_index = iter->second.ufID;
            int root = uf.find(key_index);
            if(uf.get_size(root) == 1){
                // this small cluster does not merge to any other small clusters
                // maybe it is a own cluster
                // or maybe all of the points in this small cluster are noise(may change in determing_border())
                bool has_core = false;
                for(unsigned int i=0; i<iter->second.data.size(); i++){
                    int which = iter->second.data[i];
                    if(m_is_core[which] == true){
                        has_core = true;
                        break;
                    }
                }
                if(!has_core)
                    root = -1;
            } // endof if(uf.get_size(root) == 1)
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int which = iter->second.data[i];
                m_labels[which] = root;
            }
        } // endof for(std::unorderedmap::iterator)
    }

    void DBSCAN_Grid::merge_clusters(){
        // TODO: dimension related function
        
        // initialize the UnionFind uf in class DBSCAN
        uf.init(m_hash_grid.size());

        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int cell_id = iter->first;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int point_id = iter->second.data[i];
                if(!m_is_core[point_id])
                    continue;

                merge_in_neighbour(point_id, cell_id);
            }
        }
        cell_label_to_point_label();
    }

    int DBSCAN_Grid::find_nearest_in_neighbour(int point_id, int center_id){
        // TODO: dimension related function
        // return the proper label of a un-clustered point
        // return -1 if this is a noise
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        // iterate on core points only
        float min_distance = m_eps_sqr;
        float which_label = -1;
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, Cell>::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.data.size(); j++){
                    int which = got->second.data[j];
                    if(!m_is_core[which])
                        continue;

                    float dist_sqr = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        float diff = cl_d(which, k) - cl_d(point_id, k);
                        dist_sqr += diff * diff;
                    }
                    if(dist_sqr < min_distance){
                        min_distance = dist_sqr;
                        which_label = m_labels[which];
                    }
                }
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
        return which_label;
    }

    void DBSCAN_Grid::determine_boarder_point(){
        for(unsigned int i=0; i<m_labels.size(); i++){
            if(m_labels[i] == -1){
                // calculate which cell is this point in
                int dx = int((cl_d(i,0) - m_min_x) / m_cell_width) + 1;
                int dy = int((cl_d(i,1) - m_min_y) / m_cell_width) + 1;
                int key = dx * (m_n_cols + 1) + dy;

                int label = find_nearest_in_neighbour(i, key);
                m_labels[i] = label;
            }
        }
    }

    void DBSCAN_Grid::print_grid_info() const{
        cout<<"-----------print hash grid-----------"<<endl;
        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int key = iter->first;
            int dx = key / (m_n_cols + 1);
            int dy = key % (m_n_cols + 1);
            cout<<"[key:"<<key<<" dx:"<<dx<<" dy:"<<dy<<"]"<<endl;
            for(unsigned int j=0; j<iter->second.data.size(); j++){
                int which = iter->second.data[j];
                cout<<"("<<cl_d(which,0)<<","<<cl_d(which,1)<<")  ";
            }
            cout<<endl;
        }
        cout<<"-------------------------------------"<<endl;
    }

    void DBSCAN_Grid::print_point_info() const{
        // this function should be called after the init of m_is_core and m_labels
        cout<<"-----------print point information-----------"<<endl;
        for(unsigned int i=0; i<cl_d.size1(); i++){
            cout<<"("<<cl_d(i,0)<<","<<cl_d(i,1)<<")     ";
            cout<<"["<<m_labels[i]<<" "<<m_is_core[i]<<"]"<<endl;
        }
        cout<<"-------------------------------------"<<endl;
    }

    // virtual function derived from class DBSCAN
    void DBSCAN_Grid::fit(){
        prepare_labels(cl_d.size1());

        hash_construct_grid();
        determine_core_point_grid();

        //float clock1 = get_clock();
        merge_clusters();
        //float clock2 = get_clock();
        //cout<<clock2 - clock1<<endl;

        determine_boarder_point();
    }

    void DBSCAN_Grid::test(){
        // currently do nothing 
        return;
    }
}
