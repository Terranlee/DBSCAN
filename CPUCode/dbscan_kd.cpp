#include "dbscan_kd.h"

namespace clustering{
    DBSCAN_KD::DBSCAN_KD(float eps, size_t min_elems) : DBSCAN(eps, min_elems){
        m_eps = eps;
    }
    DBSCAN_KD::~DBSCAN_KD(){
        delete root;
    }

    void DBSCAN_KD::build_tree(){
        int features = cl_d.size2();
        int N = cl_d.size1();

        // prepare the data that is needed for the kdtree
        realdata.resize(boost::extents[N][features]);
        for(int i=0; i<N; i++)
            for(int j=0; j<features; j++)
                realdata[i][j] = cl_d(i, j);

        root = new kdtree2(realdata,true);
        pos.resize(features);
    }

    void DBSCAN_KD::expand_cluster_kdtree(std::queue<int32_t>& iteration, const int cluster_id, const int pid){
        m_labels[pid] = cluster_id;
        kdtree2_result_vector result;

        while(iteration.size() != 0){
            int32_t nPid = iteration.front();
            iteration.pop();
            m_deduplicate.erase(nPid);
            if ( !m_visited[nPid] ){
                m_visited[nPid] = 1;

                for(unsigned int i=0; i<cl_d.size2(); i++)
                    pos[i] = cl_d(nPid, i);
                root->r_nearest(pos, m_eps_sqr, result);

                if(result.size() > m_min_elems){
                    for(unsigned int i=0; i<result.size(); i++){
                        int id = result[i].idx;
                        if(m_deduplicate.find(id) == m_deduplicate.end()){
                            iteration.push(id);
                            m_deduplicate.insert(id);
                        }
                    }
                }
            }
            if ( m_labels[nPid] == -1 )
                m_labels[nPid] = cluster_id;
        }
    }

    void DBSCAN_KD::dbscan_kdtree(){
        m_visited.resize(cl_d.size1(), 0);
        int32_t cluster_id = 0;
        for (uint32_t pid = 0; pid < cl_d.size1(); ++pid){
            if (!m_visited[pid]){
                m_visited[pid] = 1;

                kdtree2_result_vector result;
                for(unsigned int i=0; i<cl_d.size2(); i++)
                    pos[i] = cl_d(pid, i);

                root->r_nearest(pos, m_eps_sqr, result);

                // use '>' here, not including the central point itself
                if(result.size() > m_min_elems){
                    std::queue<int32_t> iteration;
                    m_deduplicate.clear();

                    for(unsigned int i=0; i<result.size(); i++){
                        int id = result[i].idx;
                        iteration.push(id);
                        m_deduplicate.insert(id);
                    }            
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
