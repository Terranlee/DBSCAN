#include "dbscan.h"
#include <algorithm>

using namespace clustering;

DBSCAN::Labels label_origin;
DBSCAN::Labels label_grid;

void test_original(const DBSCAN::ClusterData& cl_d){
    DBSCAN dbs(2, 3);

    double begin = DBSCAN::get_clock();
    dbs.fit_distance_matrix( cl_d );
    double end = DBSCAN::get_clock();
    std::cout<<"time is : "<<end - begin<<std::endl;

    dbs.output_result(cl_d, "output_origin");
    DBSCAN::Labels lbo = dbs.get_labels();
    label_origin.resize(lbo.size());
    std::copy(lbo.begin(), lbo.end(), label_origin.begin());
}

void test_grid(const DBSCAN::ClusterData& cl_d){
    DBSCAN dbs(2, 3);

    double begin = DBSCAN::get_clock();
    dbs.fit_grid_based(cl_d);
    double end = DBSCAN::get_clock();
    std::cout<<"time is : "<<end - begin<<std::endl;

    dbs.output_result(cl_d, "output_grid");
    DBSCAN::Labels lbg = dbs.get_labels();
    label_grid.resize(lbg.size());
    std::copy(lbg.begin(), lbg.end(), label_grid.begin());
}

int main()
{
	//DBSCAN::ClusterData cl_d = DBSCAN::gen_cluster_data( 2, 10000 );
    DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 312, "../../data/3spiral.txt");

    test_original(cl_d);
	test_grid(cl_d);

    DBSCAN::cmp_result(label_origin, label_grid);

	return 0;
}
