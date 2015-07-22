#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <vector>

#include "dbscan_matrix.h"

namespace clustering{

    DBSCAN_Matrix::DBSCAN_Matrix(float eps, size_t min_elems) : DBSCAN(eps, min_elems){}
    DBSCAN_Matrix::~DBSCAN_Matrix(){}
    
    const DBSCAN::DistanceMatrix DBSCAN_Matrix::calc_dist_matrix(){
        // rows x rows
        DBSCAN::DistanceMatrix d_m( cl_d.size1(), cl_d.size1() );
        for (size_t i = 0; i < cl_d.size1(); ++i){
            for (size_t j = i; j < cl_d.size1(); ++j){
                d_m(i, j) = 0.0;
                if (i != j){
                    ublas::matrix_row<DBSCAN::ClusterData> U (cl_d, i);
                    ublas::matrix_row<DBSCAN::ClusterData> V (cl_d, j);
					// icpc 12.1.4 does not support auto
                    //for (const auto e : ( U-V ) )
                    //    d_m(i, j) += e * e;
					for(unsigned int k=0; k<U.size(); k++)
						d_m(i, j) += (U[k] - V[k]) * (U[k] - V[k]);
                    d_m(j, i) = d_m(i, j);
                }
            }
        }
        return d_m;
    }

    DBSCAN::Neighbors DBSCAN_Matrix::find_neighbors_distance_matrix(const DBSCAN::DistanceMatrix & D, uint32_t pid){
        Neighbors ne;
        for (uint32_t j = 0; j < D.size1(); ++j){
            if( D(pid, j) < m_eps_sqr )
                ne.push_back(j);
        }
        return ne;
    }

    void DBSCAN_Matrix::expand_cluster_distance_matrix(DBSCAN::Neighbors & ne, const DBSCAN::DistanceMatrix & dm, const int cluster_id, const int pid){
        m_labels[pid] = cluster_id;
        for (uint32_t i = 0; i < ne.size(); ++i){
            uint32_t nPid = ne[i];
            if ( !m_visited[nPid] ){
                m_visited[nPid] = 1;
                Neighbors ne1 = find_neighbors_distance_matrix(dm, nPid);
                // use '>' here, not including the central point itself
                if ( ne1.size() > m_min_elems){
					// icpc 12.1.4 does not support auto
                    //for (const auto & n1 : ne1)
                    //    ne.push_back(n1);
					for(unsigned int k=0; k<ne1.size(); k++)
						ne.push_back(ne1[k]);
                }
            }
            if ( m_labels[nPid] == -1 )
                m_labels[nPid] = cluster_id;
        }
    }
    
    void DBSCAN_Matrix::dbscan_distance_matrix( const DBSCAN::DistanceMatrix & dm ){
        m_visited.resize(dm.size1(), 0);
        uint32_t cluster_id = 0;
        for (uint32_t pid = 0; pid < dm.size1(); ++pid){
            if ( !m_visited[pid] ){  
                m_visited[pid] = 1;
                Neighbors ne = find_neighbors_distance_matrix(dm, pid );
                // use '>' here, not including the central point itself
                if (ne.size() > m_min_elems){
                    expand_cluster_distance_matrix(ne, dm, cluster_id, pid);
                    ++cluster_id;
                }
            }
        }
    }

    // virtual function derived from DBSCAN
    void DBSCAN_Matrix::fit() {
        prepare_labels( cl_d.size1() );
        const DBSCAN::DistanceMatrix D = calc_dist_matrix();
        dbscan_distance_matrix( D );
    }

    void DBSCAN_Matrix::test(){
        // currently do nothing 
        return;
    }
}
