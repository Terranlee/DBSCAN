#include "dbscan.h"

using namespace clustering;

int main()
{
	//DBSCAN::ClusterData cl_d = DBSCAN::gen_cluster_data( 2, 10000 );
    DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 5000, "../../data/s1.txt");

	DBSCAN dbs(20000, 5);
	dbs.fit_distance_matrix( cl_d );
    dbs.output_result(cl_d, "output");

	return 0;
}
