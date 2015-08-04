#ifndef __DBSCAN_KD_H__
#define __DBSCAN_KD_H__

#include "kdtree.h"
#include <iostream>
#include <queue>

#include "dbscan.h"

namespace clustering{
    class DBSCAN_KD : public DBSCAN{
    public:
        DBSCAN_KD(float eps, size_t min_elems);
        virtual ~DBSCAN_KD();
        
        virtual void fit();
        virtual void test();

    protected:
        /*****************************************************************************************/
        // Variables and functions for KD Tree method
        // The algorithm is the original version of DBSCAN in the paper in KDD-96
        // Use this version as a baseline in high dimension and large dataset
        // Implemented in dbscan_kd.cpp
        std::vector<uint8_t> m_visited;
        std::vector<int> data;
        float m_eps;

        kdtree* root;
        float* pos;

        void build_tree();

        kdres* find_neighbors_kdtree(uint32_t pid);
        void expand_cluster_kdtree(std::queue<uint32_t>& iteration, const int cluster_id, const int pid);
        void dbscan_kdtree();

    };

}
#endif

