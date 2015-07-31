#include <iostream>
#include <time.h>
#include <cmath>
#include <fstream>

#include "dbscan_lsh.h"

namespace clustering{
    DBSCAN_LSH::DBSCAN_LSH(float eps, size_t min_elems) : DBSCAN_Reduced(eps, min_elems){}
    DBSCAN_LSH::~DBSCAN_LSH(){}
    
    void DBSCAN_LSH::hash_set_dimensions(){
        // the function matrix has dout lines, and din rows
        // the line of the matrix can be used to do projection
        m_hash = Functions(DOUT, cl_d.size2());
        srand(unsigned(time(NULL)));
        //srand(0);
    }

    void DBSCAN_LSH::hash_generate(){
        // may be changed to srand((unsigned) time(NULL))
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
        m_new_cell_width = eps * 0.6;
    }

    void DBSCAN_LSH::rehash_data_projection(){
        // use locality sensitive hashing to reassign the data to another grid
        // select a random data as the 
        for(unsigned int red = 0; red < REDUNDANT; red++){
            int rnd = rand() % cl_d.size1();
            for(unsigned int i=0; i<DOUT; i++){
                float mini = 0.0f;
                for(unsigned int j=0; j<cl_d.size2(); j++)
                    mini += cl_d(rnd, j) * m_hash(i, j);
                m_new_min_val[red][i] = mini;
            }
        }

        std::vector<int> temp(DOUT);
        std::vector<float> mult(DOUT);
        for(unsigned int i=0; i<cl_d.size1(); i++){
            // make projection
            for(unsigned int j=0; j<DOUT; j++){
                float data = 0.0f;
                for(unsigned int k=0; k<cl_d.size2(); k++)
                    data += cl_d(i, k) * m_hash(j, k);
                mult[j] = data;
            }
            // calculate index in each dimension
            for(unsigned int red = 0; red < REDUNDANT; red++){
                for(unsigned int j=0; j<DOUT; j++)
                    temp[j] = int((mult[j] - m_new_min_val[red][j]) / m_new_cell_width) + 1;
                // make final hash
                DimType ans(0,0);
                for(unsigned int j=0; j<DOUT/2; j++){
                    ans.first += temp[j];
                    ans.first = ans.first << 16;
                }
                for(unsigned int j=DOUT/2; j<DOUT; j++){
                    ans.second += temp[j];
                    ans.second = ans.second << 16;
                }
                m_new_grid[red][i] = ans;
            }
        }
    }

    int DBSCAN_LSH::merge_after_projection(){
        // if the points are in the same cell in the new grid in DOUT space
        // their clusters should be merged together in the original space

        // this function is similar to DBSCAN_Rehash::rehash_data
        int total_merge_counter = 0;
        for(unsigned int red=0; red<REDUNDANT; red++){
            int begin = uf.get_count();
            MergeMap merge_map;
            for(unsigned int i=0; i<cl_d.size1(); i++){
                if(!m_is_core[i])
                    continue;

                DimType key = m_new_grid[red][i];
                MergeMap::iterator got = merge_map.find(key);
                if(got == merge_map.end()){
                    std::vector<int> intvec;
                    intvec.push_back(i);
                    merge_map.insert(std::make_pair(key, intvec));
                }
                else{
                    for(unsigned j=0; j<got->second.size(); j++){
                        int id1 = got->second[j];
                        float dist = 0.0;
                        for(unsigned int k=0; k<cl_d.size2(); k++){
                            float diff = cl_d(id1, k) - cl_d(i, k);
                            dist += diff * diff;
                        }
                        if(dist <= m_eps_sqr){
                            int belong_id = m_point_to_uf[id1];
                            int center_id = m_point_to_uf[i];
                            uf.make_union(belong_id, center_id);
                            break;
                        }
                    }
                    got->second.push_back(i);
                }
            }
            int diff = begin - uf.get_count();
            //cout<<"merge : "<<diff<<" clusters"<<endl;
            total_merge_counter += diff;
        }
        cout<<endl;
        return total_merge_counter;
    }

    void DBSCAN_LSH::merge_clusters_lsh(){
        m_new_min_val.resize(REDUNDANT);
        m_new_grid.resize(REDUNDANT);
        for(unsigned int i=0; i<REDUNDANT; i++){
            m_new_min_val[i].resize(DOUT);
            m_new_grid[i].resize(cl_d.size1());
        }
        uf.init(cl_d.size1());

        m_point_to_uf.resize(cl_d.size1());
        for(std::unordered_map<HashType, Cell>::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
            int ufid = iter->second.ufID;
            for(unsigned int i=0; i<iter->second.data.size(); i++){
                int which = iter->second.data[i];
                m_point_to_uf[which] = ufid;
            }
        }

        calculate_new_width();
        hash_set_dimensions();
        // TODO:
        // currently exclude the un_core points during the merge step
        // later they should be excluded during the data preparation step
        for(int i=0; i<50; i++){

            hash_generate();
            rehash_data_projection();
            int merge_counter = merge_after_projection();
            
            if(merge_counter < int(3 * REDUNDANT)){
                cout<<"after "<<i<<" iterations, algorithm stop"<<endl;
                break;
            }
        }

        cell_label_to_point_label();
    }

    void DBSCAN_LSH::fit(){
        prepare_labels(cl_d.size1());



        float begin;
        begin = get_clock();
        hash_construct_grid();
        cout<<get_clock() - begin<<endl;

        begin = get_clock();
        determine_core_point_grid();
        cout<<get_clock() - begin<<endl;

        // the merge clusters step is the most time consuming
        // try to improve the performance of this part
        begin = get_clock();
        merge_clusters_lsh();
        cout<<get_clock() - begin<<endl;

        begin = get_clock();
        determine_boarder_point();
        cout<<get_clock() - begin<<endl;
    }

    void DBSCAN_LSH::test(){
        // currently do nothing
        return;
    }
}
