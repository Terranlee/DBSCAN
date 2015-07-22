#include "dbscan_matrix.h"
#include "dbscan_grid.h"
#include "dbscan_reduced.h"
#include "dbscan_dfe.h"

#include <algorithm>

using namespace clustering;

void test_original(DBSCAN::Labels& label_origin){

    //DBSCAN dbs(2, 3);   // the papameter for 3spiral.txt
    //DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 312, "../../data/3spiral.txt");

    DBSCAN* dbs = new DBSCAN_Matrix(20000, 4);   // the papameter for s1.txt
    dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    //DBSCAN dbs(10000, 4);
    //dbs.read_cluster_data(2, 25000, "data/5times_s1.txt");

    cout<<"start execution of distance matrix DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_origin");

    DBSCAN::Labels lbo = dbs->get_labels();
    label_origin.resize(lbo.size());
    std::copy(lbo.begin(), lbo.end(), label_origin.begin());
    cout<<endl;
    delete dbs;
}

void test_grid(DBSCAN::Labels& label_grid){
    
    //DBSCAN dbs(2, 3);   // the papameter for 3spiral.txt
    //DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 312, "../../data/3spiral.txt");

    DBSCAN* dbs = new DBSCAN_Grid(20000, 4);   // the papameter for s1.txt
    dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    //DBSCAN dbs(14000, 20);
    //dbs.read_cluster_data(2, 25000, "data/5times_s1.txt");

    cout<<"start execution of grid based DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_grid");

    DBSCAN::Labels lbg = dbs->get_labels();
    label_grid.resize(lbg.size());
    std::copy(lbg.begin(), lbg.end(), label_grid.begin());
    cout<<endl;
    delete dbs;
}

void test_reduced(DBSCAN::Labels& label_reduced){
    
    //DBSCAN dbs(2, 3);   // the papameter for 3spiral.txt
    //DBSCAN::ClusterData cl_d = DBSCAN::read_cluster_data(2, 312, "../../data/3spiral.txt");

    DBSCAN* dbs = new DBSCAN_Reduced(20000, 4);   // the papameter for s1.txt
    dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    //DBSCAN dbs(14000, 20);
    //dbs.read_cluster_data(2, 25000, "data/5times_s1.txt");

    cout<<"start execution of reduced grid based DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_grid");

    DBSCAN::Labels lbr = dbs->get_labels();
    label_reduced.resize(lbr.size());
    std::copy(lbr.begin(), lbr.end(), label_reduced.begin());
    cout<<endl;
    delete dbs;
}

void test_dfe(){

    DBSCAN* dbs = new DBSCAN_DFE(20000, 4);   // the papameter for s1.txt
    dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    cout<<"start execution of dfe DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

}

int main()
{
    /*
    //DBSCAN::Labels label_origin;
    DBSCAN::Labels label_grid;
    DBSCAN::Labels label_reduced;

    //test_original(label_origin);
    test_grid(label_grid);
    test_reduced(label_reduced);

    DBSCAN::cmp_result(label_reduced, label_grid);
    */

    test_dfe();
    return 0;
}
