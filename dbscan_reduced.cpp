#include <iostream>
#include <climits>
#include <algorithm>

#include "dbscan.h"

namespace clustering{

    void DBSCAN::process_vector(const DBSCAN::ClusterData& cl_d, std::vector<int>& vec){
        double mid_x = 0.0;
        double mid_y = 0.0;
        for(unsigned int i=0; i<vec.size(); i++){
            int which = vec[i];
            mid_x += cl_d(which, 0);
            mid_y += cl_d(which, 1);
        }
        mid_x = mid_x / (double) vec.size();
        mid_y = mid_y / (double) vec.size();

        std::vector<double> diff(vec.size());
        for(unsigned int i=0; i<vec.size(); i++)
            diff[i] = 
    }

    // making the maximum number of points in each cell is max_num_point
    // do this by remove the points that are in the center of a cell
    void DBSCAN::reduce_precision(const DBSCAN::ClusterData& cl_d, int max_num_point){
        m_max_num_point = max_num_point;
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            if(iter->second.size() <= max_num_point)
                continue;
            process_vector(cl_d, iter->second);
        }
    }

}

