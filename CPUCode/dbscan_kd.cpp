#include "dbscan_kd.h"

namespace clustering{
    DBSCAN_KD::DBSCAN_KD(float eps, size_t min_elems) : DBSCAN(eps, min_elems){
        m_eps = eps;
    }
    DBSCAN_KD::~DBSCAN_KD(){
        kd_free(root);
        delete[] pos;
    }

    void DBSCAN_KD::permute(std::vector<int>& intvec){
        srand(unsigned(time(NULL)));
        unsigned int sz = intvec.size();
        for(unsigned int i=0; i<sz; i++){
            int index = rand() % (sz - i) + i;
            int temp = intvec[i];
            intvec[i] = intvec[index];
            intvec[index] = temp;
        }
    }

    void DBSCAN_KD::build_tree(){
        int features = cl_d.size2();
        root = kd_create(features);
        pos = new float[features];

        data.resize(cl_d.size1());
        for(unsigned int i=0; i<data.size(); i++)
            data[i] = i;

        std::vector<int> which(cl_d.size1());
        for(unsigned int i=0; i<which.size(); i++)
            which[i] = i;
        permute(which);

        for(unsigned int i=0; i<cl_d.size1(); i++){
            int id = which[i];
            for(unsigned int j=0; j<cl_d.size2(); j++)
                pos[j] = cl_d(id, j);
            kd_insertf(root, pos, &data[id]);
        }
    }

    void DBSCAN_KD::expand_cluster_kdtree(std::queue<uint32_t>& iteration, const int cluster_id, const int pid){
        m_labels[pid] = cluster_id;
        while(iteration.size() != 0){
            uint32_t nPid = iteration.front();
            iteration.pop();
            m_deduplicate.erase(nPid);
            if ( !m_visited[nPid] ){
                m_visited[nPid] = 1;
                kdres* ret = find_neighbors_kdtree(nPid);

                // use '>' here, not including the central point itself
                if ( kd_res_size(ret) > (int)m_min_elems){
                    while( !kd_res_end( ret ) ){
                        uint32_t id = *(uint32_t*)kd_res_itemf( ret, pos );
                        if(m_deduplicate.find(id) == m_deduplicate.end()){
                            iteration.push(id);
                            m_deduplicate.insert(id);
                        }
                        kd_res_next( ret );
                    }
                }
                kd_res_free(ret);
            }
            if ( m_labels[nPid] == -1 )
                m_labels[nPid] = cluster_id;
        }
    }

    kdres* DBSCAN_KD::find_neighbors_kdtree(uint32_t pid){
        // release the kdres after you use it
        for(unsigned int j=0; j<cl_d.size2(); j++)
            pos[j] = cl_d(pid, j);
        kdres* ret = kd_nearest_rangef(root, pos, m_eps);
        return ret;
    }


    void DBSCAN_KD::dbscan_kdtree(){
        m_visited.resize(cl_d.size1(), 0);
        uint32_t cluster_id = 0;
        for (uint32_t pid = 0; pid < cl_d.size1(); ++pid){
            if (!m_visited[pid]){
                m_visited[pid] = 1;
                kdres* ret = find_neighbors_kdtree(pid);

                std::queue<uint32_t> iteration;
                m_deduplicate.clear();
                while( !kd_res_end( ret ) ){
                    uint32_t id = *(uint32_t*)kd_res_itemf( ret, pos );
                    iteration.push(id);
                    m_deduplicate.insert(id);
                    kd_res_next( ret );
                }
                kd_res_free(ret);

                // use '>' here, not including the central point itself
                if(iteration.size() > m_min_elems){
                    expand_cluster_kdtree(iteration, cluster_id, pid);
                    ++cluster_id;
                }
            }
        }
    }

    void DBSCAN_KD::fit(){
        float begin;
        begin = get_clock();
        build_tree();
        prepare_labels( cl_d.size1() );
        cout<<get_clock() - begin<<endl;

        begin = get_clock();
        dbscan_kdtree();
        cout<<get_clock() - begin<<endl;
    }

    void DBSCAN_KD::test(){
        // currently do nothing here
        return;
    }
}
