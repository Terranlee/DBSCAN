#ifndef __DBSCAN_DFE_H__
#define __DBSCAN_DFE_H__

#include "dbscan_grid.h"

// for Maxeler Dataflow Engine
// unable to compile without the special compiler
#include "DBSCAN.h"
#include "MaxSLiCInterface.h"

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
        unsigned int m_reduced_num;
        float* input_data;
        uint32_t* merge_answer_dfe;
        uint32_t* merge_answer_cpu;
        // we have to record the num of cells
        // because the length of input/output stream must be divide by 16bytes
        // so m_num_cells may be different from the original number of cells
        int m_num_cells;
        
        // the handle for max file/ max engine, and its related functions
        max_file_t* mf;
        max_engine_t* me;
        bool check_parameters();
        bool prepare_max_file();
        void release_max_file();

        // process_vector function is the same as the reduced precision algorithm
        // use the following two functions to reduce the precision and limit the length of iteration
        void process_vector(std::vector<int>& vec);
        void set_reduced_precision(unsigned int reduced_num);

        void prepare_data();
        void merge_clusters_dfe();
        void decode_merge_answer(uint32_t* merge_answer);
        void test_results();

        // two simulation function on cpu, trying to make the same result as the dataflow engine
        void merge_clusters_cpu();
        void merge_neighbour_cpu(int center_key, int point_id);
    };
}

#endif
