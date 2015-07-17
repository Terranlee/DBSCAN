#include "dbscan.h"
#include <algorithm>

using namespace clustering;

void test_original(DBSCAN::Labels& label_origin){
    //DBSCAN::ClusterData cl_d = DBSCAN::gen_cluster_data( 2, 10000 );

    //DBSCAN dbs(2, 3);   // the papameter for 3spiral.txt
    //DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 312, "../../data/3spiral.txt");

    //DBSCAN dbs(20000, 4);   // the papameter for s1.txt
    //dbs.read_cluster_data(2, 5000, "../../data/s1.txt");

    DBSCAN dbs(10000, 4);
    dbs.read_cluster_data(2, 25000, "data/5times_s1.txt");

    cout<<"start execution of distance matrix DBSCAN"<<endl;
    double begin = DBSCAN::get_clock();
    dbs.fit_distance_matrix();
    double end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs.reshape_labels();
    dbs.output_result("output_origin");
    DBSCAN::Labels lbo = dbs.get_labels();
    label_origin.resize(lbo.size());
    std::copy(lbo.begin(), lbo.end(), label_origin.begin());
    cout<<endl;
}

void test_grid(DBSCAN::Labels& label_grid){
    //DBSCAN::ClusterData cl_d = DBSCAN::gen_cluster_data( 2, 10000 );
    
    //DBSCAN dbs(2, 3);   // the papameter for 3spiral.txt
    //DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 312, "../../data/3spiral.txt");

    //DBSCAN dbs(20000, 4);   // the papameter for s1.txt
    //dbs.read_cluster_data(2, 5000, "../../data/s1.txt");

    DBSCAN dbs(14000, 20);
    dbs.read_cluster_data(2, 25000, "data/5times_s1.txt");

    cout<<"start execution of grid based DBSCAN"<<endl;
    double begin = DBSCAN::get_clock();
    dbs.fit_grid_based();
    double end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs.reshape_labels();
    dbs.output_result("output_grid");
    DBSCAN::Labels lbg = dbs.get_labels();
    label_grid.resize(lbg.size());
    std::copy(lbg.begin(), lbg.end(), label_grid.begin());
    cout<<endl;
}

void test_reduced(DBSCAN::Labels& label_reduced){
    //DBSCAN::ClusterData cl_d = DBSCAN::gen_cluster_data( 2, 10000 );
    
    //DBSCAN dbs(2, 3);   // the papameter for 3spiral.txt
    //DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 312, "../../data/3spiral.txt");

    //DBSCAN dbs(20000, 4);   // the papameter for s1.txt
    //dbs.read_cluster_data(2, 5000, "../../data/s1.txt");

    DBSCAN dbs(14000, 20);
    dbs.read_cluster_data(2, 25000, "data/5times_s1.txt");

    cout<<"start execution of grid based DBSCAN"<<endl;
    double begin = DBSCAN::get_clock();
    dbs.fit_grid_reduced_precision();
    double end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl<<endl;

    dbs.reshape_labels();
    dbs.output_result("output_grid");
    DBSCAN::Labels lbr = dbs.get_labels();
    label_reduced.resize(lbr.size());
    std::copy(lbr.begin(), lbr.end(), label_reduced.begin());
}

int main()
{
    //DBSCAN::Labels label_origin;
    DBSCAN::Labels label_grid;
    DBSCAN::Labels label_reduced;

    //test_original(label_origin);
    test_grid(label_grid);
    test_reduced(label_reduced);

    DBSCAN::cmp_result(label_reduced, label_grid);

    return 0;
}
