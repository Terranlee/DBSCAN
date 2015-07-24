#include <iostream>
#include <climits>
#include <algorithm>
#include <functional>
#include <cstdio>

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

        for(unsigned int i=0; i<vec.size(); i++)
            vec[i] = diff[i].second;
    }

    void DBSCAN_DFE::set_reduced_precision(unsigned int reduced_num){
        m_reduced_num = reduced_num;
    }
        
    // check the parameters from CPU and DFE are the same
    // otherwise it will not get correct result
    bool DBSCAN_DFE::check_parameters(){
        // the number of columns must be the same in dfe and cpu, or the 5*5 matrix will be wrong
        int dfe_num_cols = max_get_constant_uint64t(mf, "numCols");
        int dfe_num_points_cell = max_get_constant_uint64t(mf, "numPointsCell");
        int dfe_num_neighbour = max_get_constant_uint64t(mf, "numNeighbour");
        if(dfe_num_cols != m_n_cols || dfe_num_points_cell != m_reduced_num || dfe_num_neighbour != 25){
            cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
            cout<<"DFE configuration failed."<<endl;
            cout<<"Parameters are as followed:"<<endl;
            cout<<"dfe_num_cols : "<<dfe_num_cols<<",   m_n_cols : "<<m_n_cols<<endl;
            cout<<"dfe_num_points_cell : "<<dfe_num_points_cell<<",   m_reduced_num : "<<m_reduced_num<<endl;
            cout<<"dfe_num_neighbour : "<<dfe_num_neighbour<<endl;
            cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
            return false;
        }
        return true;
    }
    
    // some preparation before using the dataflow engine
    // prepare the max file, load to engine only when pass the parameter check
    bool DBSCAN_DFE::prepare_max_file(){
        cout<<"----------loading DFE---------"<<endl;
        mf = DBSCAN_init();
        bool check = check_parameters();
        if(!check)
            return false;
        me = max_load(mf, "*");
        cout<<"----------loading DFE finished----------"<<endl;
        return true;
    }

    // release the dataflow engine
    void DBSCAN_DFE::release_max_file(){
        max_unload(me);
    }
    
    void DBSCAN_DFE::decode_merge_answer(uint32_t* merge_answer){
        uf.init(m_hash_grid.size());

        // here in the decode procedure, we don't have to set num_neighbour to 25
        // and the iteration can start from 1, ignoring the left-up corner
        const int num_neighbour = 24;
        for(int index=0; index<m_num_cells; index++){
            int dx = index / m_n_cols;
            int dy = index % m_n_cols;
            int key = (dx - 1) * (m_n_cols + 1) + dy + 1;
            std::unordered_map<int, Cell>::const_iterator got = m_hash_grid.find(key);
            if(got == m_hash_grid.end())
                continue;

            uint32_t ans = merge_answer[index];
            int uf_center_index = got->second.ufID;
            int cell_iter = key - 2 * (m_n_cols + 1) - 1;

            const uint32_t flag = 0x40000000;
            for(int j=1; j<num_neighbour; j++){
                uint32_t which = ans & flag;
                if(which){
                    int uf_cell_index = m_hash_grid.find(cell_iter)->second.ufID;
                    uf.make_union(uf_cell_index, uf_center_index);
                }
                ans = ans << 1;
                switch(j){
                    case 4:
                        cell_iter = key - (m_n_cols + 1) - 2;
                        break;
                    case 9:
                        cell_iter = key - 2;
                        break;
                    case 14:
                        cell_iter = key + (m_n_cols + 1) - 2;
                        break;
                    case 19:
                        cell_iter = key + (m_n_cols + 1) * 2 - 2;
                        break;
                    default:
                        cell_iter = cell_iter + 1; 
                } //endof switch
            } //endof inner loop of neighbour
        } //endof outer loop, num_cell iteration

        cell_label_to_point_label();
    }

    void DBSCAN_DFE::merge_clusters_dfe(){
        // call dataflow engine computing
        int num_points = m_num_cells * m_reduced_num;
        float invalid = std::numeric_limits<float>::max();

        DBSCAN_actions_t actions;
        actions.param_N = num_points;
        actions.param_sqrEps = m_eps_sqr;
        actions.param_invalidData = invalid;
        actions.instream_input_cpu = input_data;
        actions.outstream_output_cpu = merge_answer_dfe;

        cout<<"---------doing dataflow computing--------"<<endl;
        DBSCAN_run(me, &actions);   
        cout<<"---------finish dataflow computing-------"<<endl;
    }

    void DBSCAN_DFE::prepare_data(){
        const float invalid_data = std::numeric_limits<float>::max();

        // add two lines of invalid data at the begining and end of input_data
        int num_cells = (m_n_rows + 4) * m_n_cols;

        // the input/output stream length must be divided by 16bytes
        // so the final length of stream may be different from the original one
        // in 2D case, the number of cells must be divided by 4
        while((num_cells % 4) != 0)
            num_cells++;
        m_num_cells = num_cells;
        int length = m_num_cells * m_reduced_num * 2;

        //cout<<"points length : "<<length<<", cell length : "<<m_num_cells<<endl;
        //cout<<"hashed cell length : "<<m_hash_grid.size()<<endl;
        input_data = new float[length];
        merge_answer_dfe = new uint32_t[m_num_cells];
        merge_answer_cpu = new uint32_t[m_num_cells];

        // add the two lines of beginning invalid data
        int begin = 2 * m_n_cols * m_reduced_num * 2;
        for(int i=0; i<begin; i++)
            input_data[i] = invalid_data;

        for(int i=2; i<m_n_rows + 2; i++){
            for(int j=0; j<m_n_cols; j++){
                // the key is hash table is calculated with start point (1,1)
                // the begining of i is 2, so i-1 will make it 1, starting from (1,1)
                int key = (i - 1) * (m_n_cols + 1) + j + 1;
                int index = (i * m_n_cols + j) * m_reduced_num * 2;

                std::unordered_map<int, Cell>::iterator got = m_hash_grid.find(key);
                if(got == m_hash_grid.end()){
                    // this grid is actually empty, add invalid data
                    for(unsigned int iter=0; iter<m_reduced_num * 2; iter++)
                        input_data[index++] = invalid_data;
                }                
                else if(got->second.size() > m_reduced_num){
                    // this grid is not empty, and num of points > m_reduced_num, must all be core points
                    process_vector(got->second);
                    for(unsigned int iter=0; iter<m_reduced_num; iter++){
                        int which = got->second.data[iter];
                        input_data[index++] = cl_d(which, 0);
                        input_data[index++] = cl_d(which, 1);
                    }
                }
                else{
                    // this grid is not empty, and num of points < m_reduced_num
                    // some points are not core point, add only core points 
                    int end = index + m_reduced_num * 2;
                    for(unsigned int iter=0; iter<got->second.data.size(); iter++){
                        int which = got->second.data[iter];
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

        // add the ending two lines of invalid data
        begin = (m_n_rows + 2) * m_n_cols * m_reduced_num * 2;
        for(int i=begin; i<length; i++)
            input_data[i] = invalid_data;
    }

    void DBSCAN_DFE::test_results(){
        // test the result from cpu and dfe, see if they are the same
        int counter = 0;
        int begin = 2 * m_n_cols;
        int end = (m_n_rows + 2) * m_n_cols;

        FILE* fp = fopen("output_diff", "w");
        for(int i=begin; i<end; i++){
            if(merge_answer_dfe[i] == merge_answer_cpu[i])
                counter++;
            else
                fprintf(fp, "%08x\t%08x\n", merge_answer_dfe[i], merge_answer_cpu[i]);
        }
        fclose(fp);

        cout<<counter<<" same over "<<end - begin<<endl;
    }

    // cpu simulation of what is doing in the dfe
    // use the following two functions to make sure that DFE is generating the correct output
    void DBSCAN_DFE::merge_neighbour_cpu(int center_key, int point_id){
        static const int num_neighbour = 25;
        int cell_iter = center_key - 2 * (m_n_cols + 1) - 2;

        int dx = center_key / (m_n_cols + 1);
        int dy = center_key % (m_n_cols + 1);
        // index is the position in merge_answer_cpu
        int index = (dx + 1) * m_n_cols + dy - 1;

        // iterate on core points only
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
                    if(dist_sqr < m_eps_sqr){
                        // use bit to represent the relationship between cells and neighbours
                        unsigned int fix = 0x00000001;
                        fix = fix << (31 - i);
                        merge_answer_cpu[index] |= fix;
                        break;
                    }
                }
            }

            switch(i){
                case 4:
                    cell_iter = center_key - (m_n_cols + 1) - 2;
                    break;
                case 9:
                    cell_iter = center_key - 2;
                    break;
                case 14:
                    cell_iter = center_key + (m_n_cols + 1) - 2;
                    break;
                case 19:
                    cell_iter = center_key + (m_n_cols + 1) * 2 - 2;
                    break;
                default:
                    cell_iter = cell_iter + 1;
            }
        }

    }

    void DBSCAN_DFE::merge_clusters_cpu(){
        for(int i=0; i<m_num_cells; i++)
            merge_answer_cpu[i] = 0;

        for(std::unordered_map<int, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int center_key = iter->first;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int point_id = iter->second.data[i];
                if(!m_is_core[point_id])
                    continue;
                merge_neighbour_cpu(center_key, point_id);
            }
        }
    }

    // virtual functions derived from DBSCAN_Grid
    void DBSCAN_DFE::fit(){
        prepare_labels(cl_d.size1());
        // this 10 is the same number as the one in DBSCANParameter.maxj
        set_reduced_precision(10);

        // step1 and step2 is the same as the grid based algorithm
        // because the most time consuming process is in step3
        hash_construct_grid();
        determine_core_point_grid();

        // prepare stream data for cpu simulation and dfe execution
        prepare_data();
        
        // the cpu simulation of what is happening on dataflow engine
        merge_clusters_cpu();
        
        // the following code is related to the dataflow engine
        bool check = prepare_max_file();
        if(!check)
            return;
        merge_clusters_dfe();
        release_max_file();
        
        // test whether cpu and dfe can get the same result
        test_results();
        
        // dataflow engine get the relationship between cell and neighbour
        // the merge process is still done on cpu
        decode_merge_answer(merge_answer_dfe);
        determine_boarder_point();
    }

    void DBSCAN_DFE::test(){
        // currently do nothing
        return;
    }

}

