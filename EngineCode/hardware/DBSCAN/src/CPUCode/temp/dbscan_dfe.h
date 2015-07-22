#ifndef __DBSCAN_DFE_H__
#define __DBSCAN_DFE_H__

#include "dbscan_grid.h"

namespace clustering{
    class DBSCAN_DFE : public DBSCAN_Grid{
    public:
        DBSCAN_DFE(double eps, size_t min_elems); : DBSCAN(eps, minPts){}
        virtual ~DBSCAN_DFE();

        virtual void fit();
        virtual void test();

    private:
        /*****************************************************************************************/
        // variables and functions for DFE implementation
        // this algorithm is mainly based on the grid algorithm
        // and also have the reduced precision functions to run on DFE
        // implemented in dbscan_dfe.cpp
        int reduced_num;
        float* input_data;
        vector<bool> is_valid;

        void set_reduced_precision(int r_num);
        void prepare_data();

        void merge_clusters_dfe();
    };
}

#endif
