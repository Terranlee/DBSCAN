#ifndef __DBSCAN_DFE_H__
#define __DBSCAN_DFE_H__

#include "dbscan_grid.h"

namespace clustering{
    class DBSCAN_DFE : public DBSCAN_Grid{
    public:

        DBSCAN_DFE(float eps, size_t min_elems);
        virtual ~DBSCAN_DFE();

        virtual void fit();
        virtual void test();

    private:
        /*****************************************************************************************/
        // variables and functions for DFE implementation
        // this algorithm is mainly based on the grid algorithm
        // and also have the reduced precision functions to run on DFE
        // implemented in dbscan_dfe.cpp
        int m_reduced_num;
        float* input_data;
        uint32_t* merge_answer_dfe;
        uint32_t* merge_answer_cpu;

        // process_vector function is the same as the reduced precision algorithm
        void process_vector(std::vector<int>& vec);
        void set_reduced_precision(int r_num);

        void prepare_data();
        void merge_clusters_dfe();
        void test_results();

        // two simulation function on cpu, trying to make the same result as the dataflow engine
        void merge_clusters_cpu();
        void merge_neighbour_cpu(int center_key, int point_id);
    };
}

#endif
