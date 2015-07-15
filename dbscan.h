#ifndef __DBSCAN_H__
#define __DBSCAN_H__

#include <vector>
#include <unordered_map>
#include <boost/numeric/ublas/matrix.hpp>

#include "util.h"

using namespace boost::numeric;
using std::cout;
using std::endl;

namespace clustering
{
    class DBSCAN
    {
    public:
        typedef ublas::vector<double> FeaturesWeights;
        typedef ublas::matrix<double> ClusterData;
        typedef ublas::matrix<double> DistanceMatrix;
        typedef std::vector<uint32_t> Neighbors;
        typedef std::vector<int32_t> Labels;

        /*****************************************************************************************/
        // functions for all DBSCAN methods
        // implemented in dbscan.cpp
        static void cmp_result(const Labels& a, const Labels & b);
        static double get_clock();

        // initialize, deconstruct and reset
        DBSCAN(double eps, size_t min_elems);
        DBSCAN();
        ~DBSCAN();
        void init(double eps, size_t min_elems);
        void reset();

        // data initialization functions
        void gen_cluster_data( size_t features_num, size_t elements_num);
        void read_cluster_data( size_t features_num, size_t elements_num, std::string filename);
        
        // output functions
        const Labels & get_labels() const;
        int get_cluster_number() const;
        void output_result(const std::string filename) const;
        void reshape_labels();

        // different kinds of algorithm
        // implemented in dbscan.cpp
        void fit_distance_matrix();
        void fit_grid_based();
        void fit_grid_reduced_precision();

        // test interface
        void test();

    private:
        /*****************************************************************************************/
        // variables and functions for all DBSCAN methods
        // implemented in dbscan.cpp
        double m_eps_sqr;
        size_t m_min_elems;
        Labels m_labels;
        ClusterData cl_d;

        void prepare_labels( size_t s );


        /*****************************************************************************************/
        // variables and functions for distance matrix method
        // implemented in dbscan_matrix.cpp
        std::vector<uint8_t> m_visited;

        const DistanceMatrix calc_dist_matrix();
        Neighbors find_neighbors_distance_matrix(const DistanceMatrix & D, uint32_t pid);
        void expand_cluster_distance_matrix(Neighbors& ne, const DistanceMatrix& dm, const int cluster_id, const int pid);
        void dbscan_distance_matrix( const DistanceMatrix & dm );


        /*****************************************************************************************/
        // variables and functions for grid DBSCAN method
        // currently for 2D only
        // implemented in dbscan_grid.cpp
        double m_cell_width;
        int m_n_rows;
        int m_n_cols;
        double m_min_x;
        double m_min_y;
        std::vector<bool> m_is_core;
        std::unordered_map<int, std::vector<int> > m_hash_grid;
        UnionFind uf;

        void grid_init(const int features_num);
        void getMinMax_grid(double* min_x, double* min_y, double* max_x, double* max_y );
        void cell_label_to_point_label(const std::unordered_map<int, int>& reverse_find);

        // check in neighbour function, only check the adjacent cells which is possible
        bool search_in_neighbour(int point_id, int cell_id);
        void merge_in_neighbour(int point_id, int cell_id, const std::unordered_map<int, int>& reverse);
        int find_nearest_in_neighbour(int point_id, int cell_id);
    
        // testing function
        void print_grid_info() const;
        void print_point_info() const;
        
        // four main steps for grid based DBSCAN clustering
        void hash_construct_grid();
        void determine_core_point_grid();
        void merge_clusters();
        void determine_boarder_point();


        /*****************************************************************************************/
        // variables and functions for reduced precision implementation
        // the number of points in each cell is fixed, making the length of iteration to be fixed
        // the precision of the following implementation will be reduced
        // implemented in dbscan_reduced.cpp
        int m_max_num_point;

        void process_vector(std::vector<int>& vec);
        void reduce_precision(int max_num_point);

        // same function as the grid based algorithm
        // only add a restriction about the number of points in each cells
        bool search_in_neighbour_reduced(int point_id, int cell_id);
        void merge_in_neighbour_reduced(int point_id, int cell_id, const std::unordered_map<int, int>& reverse);
        int find_nearest_in_neighbour_reduced(int point_id, int cell_id);

        void determine_core_point_grid_reduced();
        void merge_clusters_reduced();
        void determine_boarder_point_reduced();

    };

    std::ostream& operator<<(std::ostream& o, DBSCAN & d);
}

#endif
