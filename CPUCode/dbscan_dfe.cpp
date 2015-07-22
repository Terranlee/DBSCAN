#include <iostream>
#include <climits>
#include <algorithm>
#include <functional>

#include "dbscan_dfe.h"

namespace clustering{

    DBSCAN_DFE::DBSCAN_DFE(float eps, size_t min_elems) : DBSCAN_Grid(eps, min_elems){}
    DBSCAN_DFE::~DBSCAN_DFE(){
        delete[] input_data;
        delete[] merge_answer_dfe;
        delete[] merge_answer_cpu;
    }

    void DBSCAN_DFE::process_vector(std::vector<int>& vec){
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

        // for(unsigned int i=0; i<m_max_num_point; i++)
        for(unsigned int i=0; i<vec.size(); i++)
            vec[i] = diff[i].second;
        //vec.resize(m_max_num_point);
    }

    void DBSCAN_DFE::set_reduced_precision(int r_num){
        m_reduced_num = r_num;
    }

    void DBSCAN_DFE::merge_clusters_cpu(){
        int num_cells = (m_n_rows + 4) * m_n_cols;
        for(int i=0; i<num_cells; i++)
            merge_answer_cpu[i] = 0;

        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){

        }
    }

    void DBSCAN_DFE::merge_clusters_dfe(){
        // call dataflow engine computing
        cout<<"doing dataflow computing"<<endl;

        // the output answer is uint32_t because of the limit in DFE
        // but actually there are only 24 cells that needed to be checked
        /*
        const int neighbour_to_check = 24;

        for(int i=3; i<m_n_rows; i++){
            for(int j=0; j<m_n_cols; j++){
                int key = (i - 2) * (m_n_cols + 1) + j + 1;
                int index = i * m_n_cols + j;

                std::unordered_map<int, std::vector<int> >::iterator got = m_hash_grid.find(key);
                if(got == m_hash_grid.end())
                    continue;

                uint32_t cell_ans = merge_answer_dfe[index];
                for(int i=0; i<neighbour_to_check; i++){

                }
            }
        }
        */
    }

    void DBSCAN_DFE::prepare_data(){
        const float invalid_data = std::numeric_limits<float>::max();

        // add two lines of invalid data at the begining and end of input_data
        int num_cells = (m_n_rows + 4) * m_n_cols;
        int length = num_cells * m_reduced_num * 2;
        cout<<"points length : "<<length<<", cell length : "<<num_cells<<endl;
        input_data = new float[length];
        merge_answer_dfe = new uint32_t[num_cells];
        merge_answer_cpu = new uint32_t[num_cells];

        // add the beginning invalid data
        int begin = 2 * m_n_cols * m_reduced_num * 2;
        for(int i=0; i<begin; i++)
            input_data[i] = invalid_data;

        for(int i=3; i<m_n_rows; i++){
            for(int j=0; j<m_n_cols; j++){
                // the key is hash table is calculated with start point (1,1)
                int key = (i - 2) * (m_n_cols + 1) + j + 1;
                int index = (i * m_n_cols + j) * m_reduced_num * 2;

                std::unordered_map<int, std::vector<int> >::iterator got = m_hash_grid.find(key);
                if(got == m_hash_grid.end()){
                    // this grid is actually empty, add invalid data
                    for(int iter=0; iter<m_reduced_num * 2; iter++)
                        input_data[index++] = invalid_data;
                }                
                else if(got->second.size() > m_reduced_num){
                    // only core points are added, because we are at merging cluster step
                    // this grid is not empty, and num of points > m_reduced_num, must all be core points
                    process_vector(got->second);
                    for(int iter=0; iter<m_reduced_num; iter++){
                        int which = got->second[iter];
                        input_data[index++] = cl_d(which, 0);
                        input_data[index++] = cl_d(which, 1);
                    }
                }
                else{
                    // this grid is not empty, and num of points < m_reduced_num
                    // some points are not core point, add only core points 
                    int end = index + m_reduced_num * 2;
                    for(unsigned int iter=0; iter<got->second.size(); iter++){
                        int which = got->second[iter];
                        if(m_is_core[which]){
                            input_data[index++] = cl_d(which, 0);
                            input_data[index++] = cl_d(which, 1);
                        }
                    }
                    for(int iter=index; iter<end; iter++)
                        input_data[iter] = invalid_data;
                }
            }// endof inner loop j
        }// endof outer loop i

        // add the ending invalid data
        begin = (m_n_rows + 2) * m_n_cols * m_reduced_num * 2;
        for(int i=begin; i<length; i++)
            input_data[i] = invalid_data;
    }

    // virtual functions derived from DBSCAN_Grid
    void DBSCAN_DFE::fit(){
        prepare_labels(cl_d.size1());
        set_reduced_precision(m_min_elems);

        hash_construct_grid();
        determine_core_point_grid();

        // these two steps are related to the dataflow engine
        prepare_data();
        merge_clusters_dfe();
        //merge_clusters_cpu();

        //determine_boarder_point();
    }

    void DBSCAN_DFE::test(){
        // currently do nothing
        return;
    }

}

