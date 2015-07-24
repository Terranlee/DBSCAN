#include <iostream>
#include <climits>
#include <algorithm>
#include <functional>

#include "dbscan_reduced.h"

namespace clustering{

    DBSCAN_Reduced::DBSCAN_Reduced(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_Reduced::~DBSCAN_Reduced(){}
    
    void DBSCAN_Reduced::process_vector(std::vector<int>& vec){
        float mid_x = 0.0;
        float mid_y = 0.0;
        for(unsigned int i=0; i<vec.size(); i++){
            int which = vec[i];
            mid_x += cl_d(which, 0);
            mid_y += cl_d(which, 1);
        }
        mid_x = mid_x / (float) vec.size();
        mid_y = mid_y / (float) vec.size();
        
        std::vector<std::pair<float, int> > diff(vec.size());
        for(unsigned int i=0; i<vec.size(); i++){
            int which = vec[i];
            float dx = cl_d(which, 0) - mid_x;
            float dy = cl_d(which, 1) - mid_y;
            diff[i] = std::make_pair(dx * dx + dy * dy, which);
        }
        std::sort(diff.begin(), diff.end(), std::greater<std::pair<float, int> >());

        for(unsigned int i=0; i<vec.size(); i++)
            vec[i] = diff[i].second;
    }

    // making the maximum number of points in each cell is max_num_point
    // do this by remove the points that are in the center of a cell
    void DBSCAN_Reduced::reduce_precision(unsigned int max_num_point){
        m_max_num_point = max_num_point;
        for(std::unordered_map<int, Cell>::iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            if(iter->second.data.size() <= max_num_point)
                continue;
            process_vector(iter->second.data);
        }
    }

    bool DBSCAN_Reduced::search_in_neighbour_reduced(int point_id, int center_id){
        // TODO: dimension related function
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;
        unsigned int counter = 0;
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, Cell>::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                // here is the difference with the grid based algorithm
                unsigned searchSize = std::min((unsigned int)got->second.data.size(), m_max_num_point);
                for(unsigned int j=0; j<searchSize; j++){
                    int which = got->second.data[j];

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

    void DBSCAN_Reduced::determine_core_point_grid_reduced(){
        m_is_core.resize(cl_d.size1(), false);
        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            //  here we use '>', because it should not include the central point itself
            if(iter->second.data.size() > m_min_elems){
                for(unsigned int i=0; i<iter->second.data.size(); i++){
                    int which = iter->second.data[i];
                    m_is_core[which] = true;
                }
            }
            else{
                int cell_id = iter->first;
                for(unsigned int i=0; i<iter->second.data.size(); i++){
                    int point_id = iter->second.data[i];
                    bool result = search_in_neighbour_reduced(point_id, cell_id);
                    m_is_core[point_id] = result;
                }
            }
        }
        //print_point_info(cl_d);
    }

    void DBSCAN_Reduced::merge_in_neighbour_reduced(int point_id, int center_id){
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        int cell_index = m_hash_grid.find(center_id)->second.ufID;
        // iterate on core points only
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, Cell>::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                // here is the difference with the grid based algorithm
                unsigned int searchSize = std::min((unsigned int)got->second.data.size(), m_max_num_point);
                for(unsigned int j=0; j<searchSize; j++){
                    int which = got->second.data[j];
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
                        //return cell_iter;
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

    void DBSCAN_Reduced::merge_clusters_reduced(){
        // TODO: dimension related function
        
        // initialize the UnionFind uf in class DBSCAN
        uf.init(m_hash_grid.size());

        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int cell_id = iter->first;
            for(unsigned int i=0; i<iter->second.data.size() && i < m_max_num_point; i++){
                int point_id = iter->second.data[i];
                if(!m_is_core[point_id])
                    continue;

                merge_in_neighbour_reduced(point_id, cell_id);
            }
        }
        cell_label_to_point_label();
    }

    int DBSCAN_Reduced::find_nearest_in_neighbour_reduced(int point_id, int center_id){
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
                // here is the difference with the grid based algorithm
                unsigned int searchSize = std::min((unsigned int)got->second.data.size(), m_max_num_point);
                for(unsigned int j=0; j<searchSize; j++){
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

    void DBSCAN_Reduced::determine_boarder_point_reduced(){
        for(unsigned int i=0; i<m_labels.size(); i++){
            if(m_labels[i] == -1){
                // calculate which cell is this point in
                int dx = int((cl_d(i,0) - m_min_x) / m_cell_width) + 1;
                int dy = int((cl_d(i,1) - m_min_y) / m_cell_width) + 1;
                int key = dx * (m_n_cols + 1) + dy;

                int label = find_nearest_in_neighbour_reduced(i, key);
                m_labels[i] = label;
            }
        }
    }

    void DBSCAN_Reduced::detect_cell_size(){
        unsigned int max_cell_size = 0;
        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            if(iter->second.data.size() > max_cell_size)
                max_cell_size = iter->second.data.size();
        }
        cout<<"the max cell size is:"<<max_cell_size<<endl;
    }

    // virtual function derived from DBSCAN_Grid
    void DBSCAN_Reduced::fit() {
        prepare_labels(cl_d.size1());

        hash_construct_grid();
        //detect_cell_size();
        
        reduce_precision(m_min_elems);

        determine_core_point_grid_reduced();
        merge_clusters_reduced();
        determine_boarder_point_reduced();
    }

    void DBSCAN_Reduced::test(){
        // currently do nothing
        return;
    }
}

