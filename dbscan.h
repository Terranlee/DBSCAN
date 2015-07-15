#include <vector>
#include <unordered_map>
#include <boost/numeric/ublas/matrix.hpp>

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

        // use union_find to record which cells belong to the same cluster
        class UnionFind{
        private:
            // first is the parent pointer, second is the size of this union_find node
            std::vector<std::pair<int, int> > union_find;
            int count;
        public:
            UnionFind();
            UnionFind(int size);
            void init(int size);
            int find(int i);
            int get_size(int i);
            void make_union(int p, int q);
            void print_union();
            void test();
        };

        // functions for both method
        static ClusterData gen_cluster_data( size_t features_num, size_t elements_num );
        static ClusterData read_cluster_data( size_t features_num, size_t elements_num, std::string filename);
        static void cmp_result(const Labels& a, const Labels & b);
        static double get_clock();

        DBSCAN(double eps, size_t min_elems);
        DBSCAN();
        ~DBSCAN();

        void init(double eps, size_t min_elems);
        void reset();
        const Labels & get_labels() const;
        void output_result(const ClusterData& cl_d, const std::string filename) const;

        // two different kinds of algorithm
        void fit_distance_matrix( const ClusterData & C );
        void fit_grid_based(const ClusterData& C);

        // test interface
        void test();

    private:
        // for both algorithms
        double m_eps_sqr;
        size_t m_min_elems;
        Labels m_labels;
        void prepare_labels( size_t s );

        // for distance matrix method
        std::vector<uint8_t> m_visited;

        const DistanceMatrix calc_dist_matrix( const ClusterData & C);
        Neighbors find_neighbors_distance_matrix(const DistanceMatrix & D, uint32_t pid);
        void expand_cluster_distance_matrix(Neighbors& ne, const DistanceMatrix& dm, const int cluster_id, const int pid);
        void dbscan_distance_matrix( const DistanceMatrix & dm );

        // for the grid implementation
        // currently for 2D only
        double m_cell_width;
        int m_n_rows;
        int m_n_cols;
        double m_min_x;
        double m_min_y;
        std::vector<bool> m_is_core;
        std::unordered_map<int, std::vector<int> > m_hash_grid;

        void grid_init(const int features_num);
        void getMinMax_grid(const ClusterData& cl_d, double* min_x, double* min_y, double* max_x, double* max_y );
        void cell_label_to_point_label(const std::unordered_map<int, int>& reverse_find, UnionFind& uf);
        // check in neighbour function, only check the adjacent cells which is possible
        bool search_in_neighbour(const ClusterData& cl_d, int point_id, int cell_id);
        int merge_in_neighbour(const ClusterData& cl_d, int point_id, int cell_id, UnionFind& uf, const std::unordered_map<int, int>& reverse);
        int find_nearest_in_neighbour(const ClusterData& cl_d, int point_id, int cell_id);
        
        // testing function
        void print_grid_info(const ClusterData& cl_d);
        void print_point_info(const ClusterData& cld);

        void hash_construct_grid(const ClusterData & cl_d);
        void determine_core_point_grid(const ClusterData& cl_d);
        void merge_clusters(const ClusterData& cl_d);
        void determine_boarder_point(const ClusterData& cl_d);
    };

    std::ostream& operator<<(std::ostream& o, DBSCAN & d);
}