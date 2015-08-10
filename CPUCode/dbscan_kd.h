#ifndef __DBSCAN_KD_H__
#define __DBSCAN_KD_H__


#include <iostream>
#include <queue>
#include <unordered_set>
#include <boost/multi_array.hpp>
#include "dbscan.h"
#include "kdtree2.hpp"

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
        std::unordered_set<int32_t> m_deduplicate;

        float m_eps;

        kdtree2* root;
        // the input data for kd tree needed to be converted to this 
        kdtree2_array realdata;

        std::vector<float> pos;

        void build_tree();

        void expand_cluster_kdtree(std::queue<int32_t>& iteration, const int cluster_id, const int pid);
        void dbscan_kdtree();

    };

}
#endif

