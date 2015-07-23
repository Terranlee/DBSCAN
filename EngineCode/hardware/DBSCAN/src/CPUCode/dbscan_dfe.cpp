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

    void DBSCAN_DFE::set_reduced_precision(unsigned int r_num){
        m_reduced_num = r_num;
    }
		
	// check the parameters from CPU and DFE are the same
	void DBSCAN_DFE::check_parameters(){
		int dfe_num_rows = max_get_constant_uint64t(mf, "numRows");
		int dfe_num_points_cell = max_get_constant_uint64t(mf, "numPointsCell");
		int dfe_num_neighbour = max_get_constant_uint64t(mf, "numNeighbour");
		if(dfe_num_rows != m_n_rows || dfe_num_points_cell != m_reduced_num || dfe_num_neighbour != 25){
			cout<<"DFE configuration failed."<<endl;
			cout<<"Parameters are as followed:"<<endl;
			cout<<"dfe_num_rows : "<<dfe_num_rows<<endl;
			cout<<"dfe_num_points_cell : "<<dfe_num_points_cell<<endl;
			cout<<"dfe_num_neighbour : "<<dfe_num_neighbour<<endl;
			exit(-1);
		}
	}
	
	// some preparation before using the dataflow engine
	void DBSCAN_DFE::prepare_max_file(){
		cout<<"----------loading DFE---------"<<endl;
		mf = DBSCAN_init();
		check_parameters();
		me = max_load(mf, "*");
		cout<<"----------loading DFE finished----------"<<endl;
	}

	// release the dataflow engine
	void DBSCAN_DFE::release_max_file(){
		max_unload(me);
	}

    void DBSCAN_DFE::merge_neighbour_cpu(int center_key, int point_id){
        static const int num_neighbour = 25;
        int cell_iter = center_key - 2 * (m_n_cols + 1) - 2;

        int dx = center_key / (m_n_cols + 1);
        int dy = center_key % (m_n_cols + 1);
        // index is the position in merge_answer_cpu
        int index = (dx + 1) * m_n_cols + dy - 1;

        // iterate on core points only
        for(int i=0; i<num_neighbour; i++){
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
            if(got != m_hash_grid.end()){
                for(unsigned int j=0; j<got->second.size(); j++){
                    int which = got->second.at(j);
                    if(!m_is_core[which])
                        continue;

                    float dist_sqr = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        float diff = cl_d(which, k) - cl_d(point_id, k);
                        dist_sqr += diff * diff;
                    }
                    if(dist_sqr < m_eps_sqr){
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
        int num_cells = (m_n_rows + 4) * m_n_cols;
        for(int i=0; i<num_cells; i++)
            merge_answer_cpu[i] = 0;

        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int center_key = iter->first;
            for(unsigned int i=0; i<iter->second.size(); i++){
                int point_id = iter->second[i];
                if(!m_is_core[point_id])
                    continue;
                merge_neighbour_cpu(center_key, point_id);
            }
        }
    }

    void DBSCAN_DFE::decode_merge_answer_cpu(){
        uf.init(m_hash_grid.size());
        std::unordered_map<int, int> reverse_find;
        // icpc 12.1.4 does not support reserve function???
        //reverse_find.reserve(m_hash_grid.size());
        int counter = 0;
        for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            reverse_find.insert(std::make_pair(iter->first, counter));
            counter++;
        }

        const int num_neighbour = 25;
        int num_cells = (m_n_rows + 4) * m_n_cols;
        for(int index=0; index<num_cells; index++){
            int dx = index / m_n_cols;
            int dy = index % m_n_cols;
            int key = (dx - 1) * (m_n_cols + 1) + dy + 1;
            std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(key);
            if(got == m_hash_grid.end())
                continue;

            uint32_t ans = merge_answer_cpu[index];
            int uf_center_index = reverse_find.find(key)->second;
            int cell_iter = key - 2 * (m_n_cols + 1) - 2;

            const uint32_t flag = 0x80000000;
            for(int j=0; j<num_neighbour; j++){
                uint32_t which = ans & flag;
                if(which){
                    int uf_cell_index = reverse_find.find(cell_iter)->second;
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

        cell_label_to_point_label(reverse_find);
    }

    void DBSCAN_DFE::merge_clusters_dfe(){
        // call dataflow engine computing
		int num_cells = (m_n_rows + 4) * m_n_cols;
		float invalid = std::numeric_limits<float>::max();

		DBSCAN_actions_t actions;
		actions.param_N = num_cells;
		actions.param_sqrEps = m_eps_sqr;
		actions.param_invalidData = invalid;
		actions.instream_input_cpu = input_data;
		actions.outstream_output_cpu = merge_answer_dfe;

        cout<<"---------doing dataflow computing--------"<<endl;
		DFE_run(me, &actions);	
		cout<<"---------finish dataflow computing-------"<<endl;
    }

    void DBSCAN_DFE::prepare_data(){
        const float invalid_data = std::numeric_limits<float>::max();

        //int counter = 0;  //for debug

        // add two lines of invalid data at the begining and end of input_data
        int num_cells = (m_n_rows + 4) * m_n_cols;
        int length = num_cells * m_reduced_num * 2;
        cout<<"points length : "<<length<<", cell length : "<<num_cells<<endl;
        cout<<"hashed cell length : "<<m_hash_grid.size()<<endl;

        input_data = new float[length];
        merge_answer_dfe = new uint32_t[num_cells];
        merge_answer_cpu = new uint32_t[num_cells];

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

                std::unordered_map<int, std::vector<int> >::iterator got = m_hash_grid.find(key);
                if(got == m_hash_grid.end()){
                    // this grid is actually empty, add invalid data
                    for(unsigned int iter=0; iter<m_reduced_num * 2; iter++)
                        input_data[index++] = invalid_data;
                }                
                else if(got->second.size() > m_reduced_num){
                    // only core points are added, because we are at merging cluster step
                    // this grid is not empty, and num of points > m_reduced_num, must all be core points
                    //counter++;  // for debug
                    process_vector(got->second);
                    for(unsigned int iter=0; iter<m_reduced_num; iter++){
                        int which = got->second[iter];
                        input_data[index++] = cl_d(which, 0);
                        input_data[index++] = cl_d(which, 1);
                    }
                }
                else{
                    // this grid is not empty, and num of points < m_reduced_num
                    // some points are not core point, add only core points 
                    //counter++;  // for debug
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

        // add the ending two lines of invalid data
        begin = (m_n_rows + 2) * m_n_cols * m_reduced_num * 2;
        for(int i=begin; i<length; i++)
            input_data[i] = invalid_data;

        //cout<<counter<<" "<<m_hash_grid.size()<<endl; //for debug
    }

    void DBSCAN_DFE::test_results(){
        // test the result from cpu and dfe, see if they are the same
        int num_cells = (m_n_rows + 4) * m_n_cols;
        int counter = 0;
        for(int i=0; i<num_cells; i++)
            if(merge_answer_dfe[i] == merge_answer_cpu[i])
                counter++;
        cout<<counter<<" same over "<<num_cells<<endl;
    }

    // virtual functions derived from DBSCAN_Grid
    void DBSCAN_DFE::fit(){
        prepare_labels(cl_d.size1());
        set_reduced_precision(m_min_elems);

        hash_construct_grid();
        determine_core_point_grid();

        prepare_data();
		
		// the cpu version of merge clusters, use as compare
        merge_clusters_cpu();
		
		// the dfe version of merge clusters, compare with cpu result
		prepare_max_file();
		merge_clusters_dfe();
		release_max_file();
		
		// test whether cpu and dfe can get the same result
		// test of dfe ends here
		test_results();
		
        // currently use the cpu result to 
		decode_merge_answer_cpu();
        determine_boarder_point();
    }

    void DBSCAN_DFE::test(){
        // currently do nothing
        return;
    }

}

