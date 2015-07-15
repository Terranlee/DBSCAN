#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>
#include <time.h>
#include <cassert>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <vector>

#include "dbscan.h"

namespace clustering{
        // the following are for grid base method
    void DBSCAN::grid_init(const int features_num){
        double sq = sqrt(double(features_num));
        double eps = sqrt(m_eps_sqr);
        m_cell_width = eps / sq;
    }

    void DBSCAN::getMinMax_grid(const DBSCAN::ClusterData& cl_d, double* min_x, double* min_y, double* max_x, double* max_y){
        // TODO: dimension related function
        double maxx, maxy, minx, miny;
        maxx = maxy = std::numeric_limits<double>::min();
        minx = miny = std::numeric_limits<double>::max();
        for(size_t i=0; i<cl_d.size1(); i++){
                if(cl_d(i,0) > maxx)
                    maxx = cl_d(i,0);
                if(cl_d(i,0) < minx)
                    minx = cl_d(i,0);
                if(cl_d(i,1) > maxy)
                    maxy = cl_d(i,1);
                if(cl_d(i,1) < miny)
                    miny = cl_d(i,1);
        }
        *max_x = maxx;
        *max_y = maxy;
        *min_x = minx;
        *min_y = miny;
    }

    void DBSCAN::hash_construct_grid(const DBSCAN::ClusterData& cl_d){
        // TODO: dimension related function
        int features_num = cl_d.size2();
        if(features_num != 2)
            cout<<"only 2D data supported now!"<<endl;
        grid_init(features_num);

        double min_x, min_y, max_x, max_y;
        getMinMax_grid(cl_d, &min_x, &min_y, &max_x, &max_y);
        cout<<endl;
        cout<<"eps_sqr:"<<m_eps_sqr<<" minpts:"<<m_min_elems<<" cell_width:"<<m_cell_width<<endl;
        cout<<"minx:"<<min_x<<" miny:"<<min_y<<" maxx:"<<max_x<<" maxy:"<<max_y<<endl;
        m_min_x = min_x;
        m_min_y = min_y;
        int nRows = int((max_x - min_x) / m_cell_width) + 1;
        int nCols = int((max_y - min_y) / m_cell_width) + 1;

        int length = cl_d.size1();
        for(int i=0; i<length; i++){
            int dx = int((cl_d(i,0) - min_x) / m_cell_width) + 1;
            int dy = int((cl_d(i,1) - min_y) / m_cell_width) + 1;
            int key = dx * (nCols + 1) + dy;

            std::unordered_map<int, std::vector<int> >::iterator got = m_hash_grid.find(key);
            if(got == m_hash_grid.end()){
                std::vector<int> intvec;
                intvec.push_back(i);
                m_hash_grid.insert(std::make_pair(key,intvec));
            }
            else
                got->second.push_back(i);
        }
        m_n_rows = nRows;
        m_n_cols = nCols;
        //cout<<"n_rows:"<<m_n_rows<<" n_cols:"<<m_n_cols<<endl;
        //print_grid_info(cl_d);
    }

    bool DBSCAN::search_in_neighbour(const ClusterData& cl_d, int point_id, int center_id){
        // TODO: dimension related function
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;
        unsigned int counter = 0;
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.size(); j++){
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

    void DBSCAN::determine_core_point_grid(const ClusterData& cl_d){
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
                    bool result = search_in_neighbour(cl_d, point_id, cell_id);
                    m_is_core[point_id] = result;
                }
            }
        }
        //print_point_info(cl_d);
    }

    void DBSCAN::merge_in_neighbour(const DBSCAN::ClusterData& cl_d, int point_id, int center_id, UnionFind& uf, const std::unordered_map<int, int>& reverse_find){
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        // iterate on core points only
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.size(); j++){
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

    void DBSCAN::cell_label_to_point_label(const std::unordered_map<int, int>& reverse_find, UnionFind& uf){
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            // key is the index in the hash_grid, key_index is the corresponding index in the union_find structure
            int key = iter->first;
            int key_index = (int)reverse_find.find(key)->second;
            int root = uf.find(key_index);
            if(uf.get_size(root) == 1){
                bool has_core = false;
                for(unsigned int i=0; i<iter->second.size(); i++){
                    int which = iter->second[i];
                    if(m_is_core[which] == true){
                        has_core = true;
                        break;
                    }
                }
                if(has_core){
                    for(unsigned int i=0; i<iter->second.size(); i++){
                        int which = iter->second[i];
                        m_labels[which] = root;
                    }
                }
                else{
                    for(unsigned int i=0; i<iter->second.size(); i++){
                        int which = iter->second[i];
                        m_labels[which] = -1;
                    }
                }
            } // endof if(uf.get_size(root) == 1)
            else{
                for(unsigned int i=0; i<iter->second.size(); i++){
                    int which = iter->second[i];
                    m_labels[which] = root;
                }
            }
        } // endof for(std::unorderedmap::iterator)
    }

    void DBSCAN::merge_clusters(const DBSCAN::ClusterData& cl_d){
        // TODO: dimension related function
        
        // initialize the UnionFind uf
        UnionFind uf;
        uf.init(m_hash_grid.size());

        // TODO: how to deal with the reverse_find structure?
        // map the cell.key to a linear number
        std::unordered_map<int, int> reverse_find;
        reverse_find.reserve(m_hash_grid.size());
        int index = 0;
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            reverse_find.insert(std::make_pair(iter->first, index));
            index++;
        }

        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int cell_id = iter->first;
            for(unsigned int i=0; i<iter->second.size(); i++){
                int point_id = iter->second[i];
                if(!m_is_core[point_id])
                    continue;

                merge_in_neighbour(cl_d, point_id, cell_id, uf, reverse_find);
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
        cell_label_to_point_label(reverse_find, uf);

        //print_point_info(cl_d);
        //uf.print_union();
    }

    int DBSCAN::find_nearest_in_neighbour(const DBSCAN::ClusterData& cl_d, int point_id, int center_id){
        // TODO: dimension related function
        // return the proper label of a un-clustered point
        // return -1 if this is a noise
        static const int num_neighbour = 21;
        int cell_iter = center_id - 2 * (m_n_cols + 1) - 1;

        // iterate on core points only
        double min_distance = std::numeric_limits<double>::max();
        double which_label = -1;
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.size(); j++){
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

    void DBSCAN::determine_boarder_point(const DBSCAN::ClusterData& cl_d){
        for(unsigned int i=0; i<m_labels.size(); i++){
            if(m_labels[i] == -1){
                // calculate which cell is this point in
                int dx = int((cl_d(i,0) - m_min_x) / m_cell_width) + 1;
                int dy = int((cl_d(i,1) - m_min_y) / m_cell_width) + 1;
                int key = dx * (m_n_cols + 1) + dy;

                int label = find_nearest_in_neighbour(cl_d, i, key);
                m_labels[i] = label;
            }
        }
    }

    void DBSCAN::print_grid_info(const DBSCAN::ClusterData& cl_d){
        cout<<"-----------print hash grid-----------"<<endl;
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int key = iter->first;
            int dx = key / (m_n_cols + 1);
            int dy = key % (m_n_cols + 1);
            cout<<"[key:"<<key<<" dx:"<<dx<<" dy:"<<dy<<"]"<<endl;
            for(unsigned int j=0; j<iter->second.size(); j++){
                int which = iter->second.at(j);
                cout<<"("<<cl_d(which,0)<<","<<cl_d(which,1)<<")  ";
            }
            cout<<endl;
        }
        cout<<"-------------------------------------"<<endl;
    }

    void DBSCAN::print_point_info(const DBSCAN::ClusterData& cl_d){
        // this function should be called after the init of m_is_core and m_labels
        cout<<"-----------print point information-----------"<<endl;
        for(unsigned int i=0; i<cl_d.size1(); i++){
            cout<<"("<<cl_d(i,0)<<","<<cl_d(i,1)<<")     ";
            cout<<"["<<m_labels[i]<<" "<<m_is_core[i]<<"]"<<endl;
        }
        cout<<"-------------------------------------"<<endl;
    }


}
