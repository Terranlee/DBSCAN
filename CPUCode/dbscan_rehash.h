#ifndef __DBSCAN_REHASH_H__
#define __DBSCAN_REHASH_H__

#include "dbscan_grid.h"

namespace clustering{
    class DBSCAN_Rehash : public DBSCAN_Grid{
    public:
        DBSCAN_Rehash(float eps, size_t min_elems);
        virtual ~DBSCAN_Rehash();

        virtual void fit();
        virtual void test();

    protected:
        /*****************************************************************************************/
        // variables and functions for rehash DBSCAN method
        // currently for 2D only
        // implemented in dbscan_rehash.cpp

        // If the cell width is eps/sqrt(2)/2, we can reduce some distance calculation
        // This is equivalent to rehash the data to another grid with offset=eps/sqrt(2)/2

        std::vector<int> m_point_to_uf;

        // rehash the data into a new grid with offset==eps/sqrt(2)/2. and then merge the data in the same cell
        // this is equivalent to the cell_width = eps/sqrt(2)/2
        // this will merge some of the neighbour cells without distance calculation
        void rehash_data_merge();


        void merge_in_neighbour_rehash(int point_id, HashType center_key);
        void merge_clusters_rehash();
    };

}

#endif

