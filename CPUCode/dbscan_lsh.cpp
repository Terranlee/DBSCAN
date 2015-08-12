#include <iostream>
#include <time.h>
#include <cmath>
#include <limits>
#include <fstream>
#include <memory.h>
#include <sstream>
#include "dbscan_lsh.h"

namespace clustering{
	// const values in this class
	const unsigned int DBSCAN_LSH::REDUNDANT = 2;
	const unsigned int DBSCAN_LSH::DOUT = 8;

    DBSCAN_LSH::DBSCAN_LSH(float eps, size_t min_elems, int num_iter) : DBSCAN_Reduced(eps, min_elems){
        m_num_iter = num_iter;
    }
    DBSCAN_LSH::~DBSCAN_LSH(){}
	
    void DBSCAN_LSH::permute(std::vector<int>& intvec){
        unsigned int sz = intvec.size();
        for(unsigned int i=0; i<sz; i++){
            int index = rand() % (sz - i) + i;
            int temp = intvec[i];
            intvec[i] = intvec[index];
            intvec[index] = temp;
        }
    }

    void DBSCAN_LSH::reduced_precision_lsh(unsigned int max_num_point){
        // reduced precision, just like in dbscan_reduced.cpp
        // but save the total number of points to m_total_num
        // this is needed during init_data_structure()
        unsigned int total_num = 0;
        m_max_num_point = max_num_point;
        m_origin_to_reduced.resize(cl_d.size1(), false);

        for(std::unordered_map<HashType, Cell>::iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            if(iter->second.data.size() <= max_num_point){
                for(unsigned int i=0; i<iter->second.data.size(); i++){
                    int which = iter->second.data[i];
                    m_origin_to_reduced[which] = true;
                }
                total_num += iter->second.data.size();
            }
            else{
                process_vector(iter->second.data);
                for(unsigned int i=0; i<max_num_point; i++){
                    int which = iter->second.data[i];
                    m_origin_to_reduced[which] = true;
                }
                total_num += max_num_point;
            }
        }
        m_total_num = total_num;

        int index = 0;
        m_reduced_to_origin.resize(m_total_num);
        for(unsigned int i=0; i<cl_d.size1(); i++)
            if(m_origin_to_reduced[i])
                m_reduced_to_origin[index++] = i;
    }

    void DBSCAN_LSH::hash_set_dimensions(){
        // the function matrix has dout lines, and din rows
        // the line of the matrix can be used to do projection
        m_hash = Functions(DOUT, cl_d.size2());
    }

    void DBSCAN_LSH::hash_generate(){
        for(unsigned int i=0; i<m_hash.size1(); i++)
            for(unsigned int j=0; j<m_hash.size2(); j++)
                m_hash(i, j) = float(rand()) / float(RAND_MAX);

        // the parameter of the hash function need to be formalized
        // dist = inner_product(point, hyperplane_parameter) / norm2(hyperplane_parameter)
        for(unsigned int i=0; i<m_hash.size1(); i++){
            float sqr_sum = 0.0f;
            for(unsigned int j=0; j<m_hash.size2(); j++)
                sqr_sum += m_hash(i, j) * m_hash(i, j);
            float sqrt_sum = std::sqrt(sqr_sum);
            for(unsigned int j=0; j<m_hash.size2(); j++)
                    m_hash(i, j) /= sqrt_sum;
        }
    }

    void DBSCAN_LSH::calculate_new_width(){
        // what is the cell width in the projection space?
        // we must make sure that not all clusters are merged in the projection space

        //float sqr_dout = std::sqrt(float(dout));
        // the cell_width in high dimension is also eps theoratically
        // but consider the possibility of wrong classification, we multiply it by 0.5, and do more iteration
        float eps = std::sqrt(m_eps_sqr);
        m_new_cell_width = eps * 1.0;
    }

    void DBSCAN_LSH::rehash_data_projection(){
        // use locality sensitive hashing to reassign the data to another grid
        // select a random data as the 
        const unsigned int cl_dsz2 = cl_d.size2();
        const unsigned int cl_dsz1 = cl_d.size1();
        for(unsigned int red = 0; red < REDUNDANT; red++){
            int rnd = rand() % cl_dsz1;
            for(unsigned int i=0; i<DOUT; i++){
                float mini = 0.0f;
                for(unsigned int j=0; j<cl_dsz2; j++)
                    mini += cl_d(rnd, j) * m_hash(i, j);
                m_new_min_val[red][i] = mini;
            }
        }

        int16_t temp[DOUT];
        std::vector<float> mult(DOUT);
        int index = 0;
        for(unsigned int i=0; i<cl_dsz1; i++){
            if(!m_origin_to_reduced[i])
                continue;

            // make projection
            for(unsigned int j=0; j<DOUT; j++){
                float data = 0.0f;
                for(unsigned int k=0; k<cl_dsz2; k++)
                    data += cl_d(i, k) * m_hash(j, k);
                mult[j] = data;
            }
            // calculate index in each dimension
            for(unsigned int red = 0; red < REDUNDANT; red++){
                for(unsigned int j=0; j<DOUT; j++)
                    temp[j] = (int16_t)((mult[j] - m_new_min_val[red][j]) / m_new_cell_width);
                memcpy(&m_new_grid[red][index], temp, sizeof(int64_t) * 2);
            }
            index++;
        }
    }

    void DBSCAN_LSH::merge_cell_after_hash(){
        for(unsigned int red=0; red<REDUNDANT; red++){
            m_merge_map[red].clear();
            const NewGrid& gridding = m_new_grid.at(red);
            MergeMap& mapping = m_merge_map.at(red);

            for(unsigned int i=0; i<m_total_num; i++){
                DimType key = gridding[i];
                MergeMap::iterator got = mapping.find(key);
                int point = m_reduced_to_origin[i];
                // hash points to the same bucket by the hash value
                if(got == mapping.end()){
                    std::vector<int> intvec;
                    intvec.push_back(point);
                    mapping.insert(std::make_pair(key, intvec));
                }
                else
                    got->second.push_back(point);
            }
            for(MergeMap::iterator iter = mapping.begin(); iter != mapping.end(); iter++){
                permute(iter->second);
            }
        }
    }

    int DBSCAN_LSH::determine_core_point_lsh_sub(CoreDetermine& cd){
        const unsigned int cdsz2 = cd.size2();
        const unsigned int cl_dsz2 = cl_d.size2();
        
        int counter = 0;    
        for(unsigned int red=0; red<REDUNDANT; red++){
            const MergeMap& mapping = m_merge_map.at(red);
            const NewGrid& gridding = m_new_grid.at(red);

            // iterate through all the points
            // find neighbours in the hash bucket if this is not a core point
            for(unsigned int i=0; i<m_total_num; i++){
                // point is the original id, i is the reduced id
                int point = m_reduced_to_origin[i];
                if(!m_is_core[point]){
                    DimType key = gridding[i];
                    MergeMap::const_iterator got = mapping.find(key);
                    int core_index = m_core_map[point];
                    int sz2 = got->second.size();
                    for(int j=0; j<sz2; j++){
                        // do distance calculation here
                        int which = got->second[j];
                        if(which == point)
                            continue;

                        float dist = 0.0f;
                        for(unsigned int k=0; k<cl_dsz2; k++){
                            float diff = cl_d(which, k) - cl_d(point, k);
                            dist += diff * diff;
                        }
                        if(dist < m_eps_sqr){
                            // if distance is less than eps, then add it to the CoreDetermine matrix
                            unsigned int k;
                            for(k=0; k<cdsz2; k++){
                                if(cd(core_index, k) == which || cd(core_index, k) == -1)
                                    break;
                            }
                            if(k == cdsz2){
                                m_is_core[point] = true;
                                counter++;
                                break;
                            }
                            else if(cd(core_index, k) == -1){
                                cd(core_index, k) = which;
                            }
                        }
                    }
                }// endof !m_is_core[i]
            }// endof for(cl_d.size1())
        }// endof REDUNDANT
        return counter;
    }

    int DBSCAN_LSH::merge_clusters_lsh_sub(){
        // if the points are in the same cell in the new grid in DOUT space
        // their clusters should be merged together in the original space

        // this function is similar to DBSCAN_Rehash::rehash_data
        int total_merge_counter = 0;
        for(unsigned int red=0; red<REDUNDANT; red++){
            int begin = uf.get_count();
            const MergeMap& mapping = m_merge_map.at(red);
            const NewGrid& grid = m_new_grid.at(red);

            for(unsigned int i=0; i<m_total_num; i++){
                // point is the original id, i is the reduced id
                int point = m_reduced_to_origin[i];
                if(!m_is_core[point])
                    continue;

                DimType key = grid[i];
                MergeMap::const_iterator got = mapping.find(key);
                int center_id = m_point_to_uf[point];
                //int center_root = uf.find(center_id);
                for(unsigned int j=0; j<got->second.size(); j++){
                    int id1 = got->second[j];
                    if(id1 == point || (!m_is_core[id1]))
                        continue;

                    float dist = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        float diff = cl_d(id1, k) - cl_d(point, k);
                        dist += diff * diff;
                    }
                    if(dist < m_eps_sqr){
                        int belong_id = m_point_to_uf[id1];
                        uf.make_union(belong_id, center_id);
                        break;
                    }
                }
            }

            int diff = begin - uf.get_count();
            cout<<"merge : "<<diff<<" clusters   :   "<<uf.get_count()<<endl;
            total_merge_counter += diff;
        }
        return total_merge_counter;
    }

    void DBSCAN_LSH::init_data_structure(){
        m_is_core.resize(cl_d.size1(), false);
        m_core_map.resize(cl_d.size1());
        for(std::unordered_map<HashType, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            //  here we use '>', because it should not include the central point itself
            if(iter->second.data.size() > m_min_elems){
                for(unsigned int i=0; i<iter->second.data.size(); i++){
                    int which = iter->second.data.at(i);
                    m_is_core[which] = true;
                }
            }
        }
        
        // initialize the redundant grid and merge
        m_new_min_val.resize(REDUNDANT);
        m_new_grid.resize(REDUNDANT);
        m_merge_map.resize(REDUNDANT);

        for(unsigned int i=0; i<REDUNDANT; i++){
            m_new_min_val[i].resize(DOUT);
            m_new_grid[i].resize(m_total_num);
        }
        calculate_new_width();
        hash_set_dimensions();

        // also initialize union find structure here
        uf.init(m_hash_grid.size());
        m_point_to_uf.resize(cl_d.size1());
        for(std::unordered_map<HashType, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int ufid = iter->second.ufID;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int which = iter->second.data[i];
                m_point_to_uf[which] = ufid;
            }
        }
    }

    void DBSCAN_LSH::main_iteration(){
        // these three functions are one iteration of hash-merge procedure
        hash_generate();
        rehash_data_projection();
        merge_cell_after_hash();
    }

    int DBSCAN_LSH::set_core_map(){
        int index = 0;
        for(unsigned int i=0; i<m_is_core.size(); i++)
            if(!m_is_core[i])
                m_core_map[i] = index++;
        return index;
    }

    void DBSCAN_LSH::determine_core_point_lsh(){
        // determine core points using the result of merge
        int index = set_core_map();
        cout<<index<<" not core"<<endl;

        CoreDetermine cd = CoreDetermine(index, m_min_elems);
        for(int i=0; i<index; i++)
            for(unsigned int j=0; j<m_min_elems; j++)
                cd(i, j) = -1;

        determine_core_point_lsh_sub(cd);
    }

    void DBSCAN_LSH::merge_clusters_lsh(){
        int index = set_core_map();
        CoreDetermine cd = CoreDetermine(index, m_min_elems);
        for(int i=0; i<index; i++)
            for(unsigned int j=0; j<m_min_elems; j++)
                cd(i, j) = -1;

        for(int i=0; i<m_num_iter; i++){
            main_iteration();
            int minus = determine_core_point_lsh_sub(cd);
            index -= minus;
            cout<<index<<" not core"<<endl;
            merge_clusters_lsh_sub();
        }

        cell_label_to_point_label();
    }

    void DBSCAN_LSH::determine_boarder_point_lsh(){
        for(unsigned int i=0; i<m_total_num; i++){
            // point is the original id, i is the reduced precision id
            int point = m_reduced_to_origin[i];
            if(m_labels[point] == -1){
                std::unordered_map<int, float>::iterator d = m_boarder_dist.find(point);
                float min_dist = d->second;

                for(unsigned int red=0; red<REDUNDANT; red++){
                    DimType key = m_new_grid[red][i];
                    MergeMap::const_iterator got = m_merge_map[red].find(key);

                    for(unsigned int j=0; j<got->second.size(); j++){
                        int which = got->second[j];
                        if(!m_is_core[which])
                            continue;
                        float dist = 0.0f;

                        for(unsigned int k=0; k<cl_d.size2(); k++){
                            float diff = cl_d(point, k) - cl_d(which,k);
                            dist += diff * diff;
                        }
                        if(dist < min_dist){
                            min_dist = dist;
                            m_labels[point] = m_labels[which];
                            d->second = dist;
                        }
                    }// endof for(got->second.size())
                }// endof for(red<REDUNDANT)
            }// endof if(m_labels[point])
        }// endof for(m_total_num)
    }

    void DBSCAN_LSH::fit(){
        srand(unsigned(time(NULL)));
        prepare_labels(cl_d.size1());

        // the construct grid method is the same as the original grid one
        float begin = get_clock();
        hash_construct_grid();
        cout<<get_clock() - begin<<endl;

        // reduced precision, prepare data structures
        begin = get_clock();
        reduced_precision_lsh(m_min_elems * 3);
        init_data_structure();
        cout<<get_clock() - begin<<endl;

        // do the first main iteration 
        // and determine core points for the first time
        // and merge clusters for the first time
        begin = get_clock();
        main_iteration();
        determine_core_point_lsh();
        merge_clusters_lsh_sub();
        cout<<get_clock() - begin<<endl;

        // iteration between determine_core_point and merge_clusters
        begin = get_clock();
        merge_clusters_lsh();
        cout<<get_clock() - begin<<endl;

        int counter = 0;
        for(unsigned int i=0; i<m_labels.size(); i++){
            if(m_labels[i] == -1)
                counter++;
        }
        cout<<"now, "<<counter<<" noise points"<<endl;

        begin = get_clock();
        for(unsigned int i=0; i<m_labels.size(); i++)
            if(m_labels[i] == -1)
                m_boarder_dist.insert(std::make_pair(i, m_eps_sqr));

        determine_boarder_point_lsh();
        for(int i=0; i<10; i++){
            main_iteration();
            determine_boarder_point_lsh();
        }
        cout<<get_clock() - begin<<endl;
        
    }

    void DBSCAN_LSH::test(){
        // currently do nothing
        return;
    }
}
