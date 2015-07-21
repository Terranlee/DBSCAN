#include <iostream>
#include <climits>
#include <algorithm>
#include <functional>

#include "dbscan.h"

namespace clustering{

    void DBSCAN::process_vector(std::vector<int>& vec){
        double mid_x = 0.0;
        double mid_y = 0.0;
        for(unsigned int i=0; i<vec.size(); i++){
            int which = vec[i];
            mid_x += cl_d(which, 0);
            mid_y += cl_d(which, 1);
        }
        mid_x = mid_x / (double) vec.size();
        mid_y = mid_y / (double) vec.size();
        
        std::vector<std::pair<double, int> > diff(vec.size());
        for(unsigned int i=0; i<vec.size(); i++){
            int which = vec[i];
            double dx = cl_d(which, 0) - mid_x;
            double dy = cl_d(which, 1) - mid_y;
            diff[i] = std::make_pair(dx * dx + dy * dy, which);
        }
        std::sort(diff.begin(), diff.end(), std::greater<std::pair<double, int> >());

        // for(unsigned int i=0; i<m_max_num_point; i++)
        for(unsigned int i=0; i<vec.size(); i++)
            vec[i] = diff[i].second;
        //vec.resize(m_max_num_point);
    }

    // making the maximum number of points in each cell is max_num_point
    // do this by remove the points that are in the center of a cell
    void DBSCAN::reduce_precision(unsigned int max_num_point){
        m_max_num_point = max_num_point;
        for(std::unordered_map<int, std::vector<int> >::iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            if(iter->second.size() <= max_num_point)
                continue;
            process_vector(iter->second);
        }
    }

    bool DBSCAN::search_in_neighbour_reduced(int point_id, int center_id){
        // TODO: dimension related function
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;
        unsigned int counter = 0;
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.size() && j < m_max_num_point; j++){
                    int which = got->second.at(j);

                    double dist_sqr = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        double diff = cl_d(which, k) - cl_d(point_id, k);
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
            cell_iter = cell_iter + 1;
            if(i == 2)          cell_iter = center_id - (m_n_cols + 1) - 2;
            else if(i == 7)     cell_iter = center_id - 2;
            else if(i == 12)    cell_iter = center_id + (m_n_cols + 1) - 2;
            else if(i == 17)    cell_iter = center_id + (m_n_cols + 1) * 2 - 1;
        }
        return false;
    }

    void DBSCAN::determine_core_point_grid_reduced(){
        m_is_core.resize(cl_d.size1(), false);
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            //  here we use '>', because it should not include the central point itself
            if(iter->second.size() > m_min_elems){
                for(unsigned int i=0; i<iter->second.size(); i++){
                    int which = iter->second.at(i);
                    m_is_core[which] = true;
                }
            }
            else{
                int cell_id = iter->first;
                for(unsigned int i=0; i<iter->second.size(); i++){
                    int point_id = iter->second.at(i);
                    bool result = search_in_neighbour_reduced(point_id, cell_id);
                    m_is_core[point_id] = result;
                }
            }
        }
        //print_point_info(cl_d);
    }

    void DBSCAN::merge_in_neighbour_reduced(int point_id, int center_id, const std::unordered_map<int, int>& reverse_find){
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        // iterate on core points only
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.size() && j < m_max_num_point; j++){
                    int which = got->second.at(j);
                    if(!m_is_core[which])
                        continue;

                    double dist_sqr = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        double diff = cl_d(which, k) - cl_d(point_id, k);
                        dist_sqr += diff * diff;
                    }
                    if(dist_sqr < m_eps_sqr){
                        int belong_index = reverse_find.find(cell_iter)->second;
                        int cell_index = reverse_find.find(center_id)->second;
                        uf.make_union(belong_index, cell_index);
                        break;
                        //return cell_iter;
                    }
                }
            }

            cell_iter = cell_iter + 1;
            if(i == 2)          cell_iter = center_id - (m_n_cols + 1) - 2;
            else if(i == 7)     cell_iter = center_id - 2;
            else if(i == 12)    cell_iter = center_id + (m_n_cols + 1) - 2;
            else if(i == 17)    cell_iter = center_id + (m_n_cols + 1) * 2 - 1;
        }
    }

    void DBSCAN::merge_clusters_reduced(){
        // TODO: dimension related function
        
        // initialize the UnionFind uf in class DBSCAN
        uf.init(m_hash_grid.size());

        // TODO: how to deal with the reverse_find structure?
        // map the cell.key to a linear number
        std::unordered_map<int, int> reverse_find;
		// icpc 12.1.4 does not support reserve function???
        //reverse_find.reserve(m_hash_grid.size());
        int index = 0;
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            reverse_find.insert(std::make_pair(iter->first, index));
            index++;
        }

        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int cell_id = iter->first;
            for(unsigned int i=0; i<iter->second.size() && i < m_max_num_point; i++){
                int point_id = iter->second[i];
                if(!m_is_core[point_id])
                    continue;

                merge_in_neighbour_reduced(point_id, cell_id, reverse_find);
                // for debug
                /*
                int dx1 = cell_id / (m_n_cols + 1);
                int dy1 = cell_id % (m_n_cols + 1);
                int dx2 = belong_id / (m_n_cols + 1);
                int dy2 = belong_id % (m_n_cols + 1);
                cout<<"("<<dx1<<","<<dy1<<")  -- ("<<dx2<<","<<dy2<<")"<<endl; 
                */
            }
        }
        cell_label_to_point_label(reverse_find);

        //print_point_info(cl_d);
        //uf.print_union();
    }

    int DBSCAN::find_nearest_in_neighbour_reduced(int point_id, int center_id){
        // TODO: dimension related function
        // return the proper label of a un-clustered point
        // return -1 if this is a noise
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        // iterate on core points only
        double min_distance = m_eps_sqr;
        double which_label = -1;
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.size() && j < m_max_num_point; j++){
                    int which = got->second.at(j);
                    if(!m_is_core[which])
                        continue;

                    double dist_sqr = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        double diff = cl_d(which, k) - cl_d(point_id, k);
                        dist_sqr += diff * diff;
                    }
                    if(dist_sqr < min_distance){
                        min_distance = dist_sqr;
                        which_label = m_labels[which];
                    }
                }
            }

            cell_iter = cell_iter + 1;
            if(i == 2)          cell_iter = center_id - (m_n_cols + 1) - 2;
            else if(i == 7)     cell_iter = center_id - 2;
            else if(i == 12)    cell_iter = center_id + (m_n_cols + 1) - 2;
            else if(i == 17)    cell_iter = center_id + (m_n_cols + 1) * 2 - 1;
        }
        return which_label;
    }

    void DBSCAN::determine_boarder_point_reduced(){
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

    void DBSCAN::detect_cell_size(){
        unsigned int max_cell_size = 0;
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            if(iter->second.size() > max_cell_size)
                max_cell_size = iter->second.size();
        }
        cout<<"the max cell size is:"<<max_cell_size<<endl;
    }

}

