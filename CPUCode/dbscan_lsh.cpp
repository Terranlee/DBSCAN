#include <iostream>
#include <time.h>
#include <cmath>
#include <limits>
#include <fstream>

#include "dbscan_lsh.h"

namespace clustering{
	// const values in this class
	const unsigned int DBSCAN_LSH::REDUNDANT = 4;
	const unsigned int DBSCAN_LSH::DOUT = 8;

    DBSCAN_LSH::DBSCAN_LSH(float eps, size_t min_elems) : DBSCAN_Reduced(eps, min_elems){}
    DBSCAN_LSH::~DBSCAN_LSH(){}

    void DBSCAN_LSH::hash_set_dimensions(){
        // the function matrix has dout lines, and din rows
        // the line of the matrix can be used to do projection
        m_hash = Functions(DOUT, cl_d.size2());
        srand(unsigned(time(NULL)));
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
        m_new_cell_width = eps * 1.0;
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

    void DBSCAN_LSH::merge_cell_after_hash(bool possible){
        for(unsigned int red=0; red<REDUNDANT; red++){
            m_merge_map[red].clear();
            for(unsigned int i=0; i<m_new_grid[red].size(); i++){
                DimType key = m_new_grid[red][i];
                MergeMap::iterator got = m_merge_map[red].find(key);

                if(possible){
                    int ufID = m_point_to_uf[i];
                    int root = uf.find(ufID);
                    if(got == m_merge_map[red].end()){
                        std::vector<int> intvec;
                        intvec.push_back(i);
                        m_merge_map[red].insert(std::make_pair(key, intvec));
                    }
                    else{
                        // merge points into a bucket 
                        // only when they have the possibility to merge small cells
                        for(unsigned int j=0; j<got->second.size(); j++){
                            int ufID1 = m_point_to_uf[got->second[j]];
                            int root1 = uf.find(ufID1);
                            if(root1 != root){
                                got->second.push_back(i);
                                break;
                            }
                        }
                    }
                }
                
                else{
                    if(got == m_merge_map[red].end()){
                        std::vector<int> intvec;
                        intvec.push_back(i);
                        m_merge_map[red].insert(std::make_pair(key, intvec));
                    }
                    else
                        got->second.push_back(i);                 
                }
            }
        }
    }

    void DBSCAN_LSH::determine_core_using_merge(int index){
        CoreDetermine cd = CoreDetermine(index, m_min_elems);
        for(unsigned int i=0; i<cd.size1(); i++)
            for(unsigned int j=0; j<cd.size2(); j++)
                cd(i, j) = -1;

        int sz1 = cl_d.size1();
        for(unsigned int red=0; red<REDUNDANT; red++){
            const MergeMap& mapping = m_merge_map.at(red);
            // iterate through all the points
            // find neighbours in the hash bucket if this is not a core point
            for(int i=0; i<sz1; i++){
                if(!m_is_core[i]){
                    DimType key = m_new_grid[red][i];
                    MergeMap::const_iterator got = mapping.find(key);
                    int core_index = m_core_map[i];
                    int sz2 = got->second.size();
                    for(int j=0; j<sz2; j++){
                        // do distance calculation here
                        int which = got->second[j];
                        if(which == i)
                            continue;

                        float dist = 0.0f;
                        for(unsigned int k=0; k<cl_d.size2(); k++){
                            float diff = cl_d(which, k) - cl_d(i, k);
                            dist += diff * diff;
                        }
                        if(dist < m_eps_sqr){
                            // if distance is less than eps, then add it to the CoreDetermine matrix
                            unsigned int k;
                            for(k=0; k<cd.size2(); k++){
                                if(cd(core_index, k) == which || cd(core_index, k) == -1)
                                    break;
                            }
                            if(cd(core_index, k) == -1){
                                cd(core_index, k) = which;
                            }
                            else if(k == cd.size2()){
                                m_is_core[i] = true;
                                break;
                            }
                        }
                    }
                }// endof !m_is_core[i]
            }// endof for(cl_d.size1())
        }// endof REDUNDANT
    }

    int DBSCAN_LSH::merge_small_clusters(){
        // if the points are in the same cell in the new grid in DOUT space
        // their clusters should be merged together in the original space

        // this function is similar to DBSCAN_Rehash::rehash_data
        int total_merge_counter = 0;
        for(unsigned int red=0; red<REDUNDANT; red++){
            int begin = uf.get_count();
            const MergeMap& mapping = m_merge_map.at(red);
            const NewGrid& grid = m_new_grid.at(red);

            for(unsigned int i=0; i<cl_d.size1(); i++){
                if(!m_is_core[i])
                    continue;

                DimType key = grid[i];
                MergeMap::const_iterator got = mapping.find(key);
                int center_id = m_point_to_uf[i];
                for(unsigned int j=0; j<got->second.size(); j++){
                    int id1 = got->second[j];
                    if(id1 == (int)i)
                        continue;

                    float dist = 0.0;
                    for(unsigned int k=0; k<cl_d.size2(); k++){
                        float diff = cl_d(id1, k) - cl_d(i, k);
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
            m_new_grid[i].resize(cl_d.size1());
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
        merge_cell_after_hash(false);
    }

    int DBSCAN_LSH::set_core_map(){
        int index = 0;
        for(unsigned int i=0; i<m_is_core.size(); i++)
            if(!m_is_core[i])
                m_core_map[i] = index++;
        return index;
    }

    void DBSCAN_LSH::determine_core_point_lsh(){
        main_iteration();
        
        // determine core points using the result of merge
        int index = set_core_map();
        cout<<"!!!!!!!"<<index<<endl;
        determine_core_using_merge(index);
    }

    void DBSCAN_LSH::merge_clusters_lsh(){
        // use the grid result in determine_core_points to do the first merge
        merge_small_clusters();

        for(int i=0; i<10; i++){
            determine_core_point_lsh();
            merge_small_clusters();
        }

        for(int i=0; i<10; i++){
            main_iteration();
            int merge_counter = merge_small_clusters();
            if(merge_counter < int(3 * REDUNDANT)){
                cout<<"after "<<i<<" iterations, algorithm stop"<<endl;
                break;
            }
        }
        cell_label_to_point_label();
        /*
        int counter = 0;
        for(unsigned int i=0; i<m_labels.size(); i++)
            if(m_labels[i] == -1)
                counter++;
        cout<<"!!!!!"<<counter<<"noise"<<endl;
        */
    }

    void DBSCAN_LSH::determine_boarder_point_lsh(){
        for(unsigned int i=0; i<m_labels.size(); i++){
            if(m_labels[i] == -1){
                for(unsigned int red=0; red<REDUNDANT; red++){
                    DimType key = m_new_grid[red][i];
                    MergeMap::const_iterator got = m_merge_map[red].find(key);

                    float min_dist = std::numeric_limits<float>::max();
                    for(unsigned int j=0; j<got->second.size(); j++){
                        int which = got->second[j];
                        if(!m_is_core[i])
                            continue;
                        float dist = 0.0f;
                        for(unsigned int k=0; k<got->second.size(); k++){
                            float diff = cl_d(i, k) - cl_d(which,k);
                            dist += diff * diff;
                        }
                        if(dist < min_dist){
                            min_dist = dist;
                            m_labels[i] = m_labels[which];
                        }
                    }
                }
            }
        }
    }

    void DBSCAN_LSH::fit(){
        prepare_labels(cl_d.size1());

        // the construct grid method is the same as the original grid one
        float begin;
        begin = get_clock();
        hash_construct_grid();
        cout<<get_clock() - begin<<endl;
    
        init_data_structure();

        begin = get_clock();
        determine_core_point_lsh();
        cout<<get_clock() - begin<<endl;

        int counter = 0;
        for(unsigned int i=0; i<m_is_core.size(); i++)
            if(m_is_core[i])
                counter++;
        cout<<counter<<" core points in "<<m_is_core.size()<<endl;

        begin = get_clock();
        merge_clusters_lsh();
        cout<<get_clock() - begin<<endl;

        begin = get_clock();
        determine_boarder_point_lsh();
        cout<<get_clock() - begin<<endl;
    }

    void DBSCAN_LSH::test(){
        // currently do nothing
        return;
    }
}
