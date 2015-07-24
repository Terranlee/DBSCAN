#include "dbscan_matrix.h"
#include "dbscan_grid.h"

#include <algorithm>

using namespace clustering;

void test_original(Labels& label_origin){

    //DBSCAN* dbs = new DBSCAN_Matrix(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    DBSCAN* dbs = new DBSCAN_Matrix(10000, 4);
    dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");

    cout<<"start execution of distance matrix DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_origin");

    Labels lbo = dbs->get_labels();
    label_origin.resize(lbo.size());
    std::copy(lbo.begin(), lbo.end(), label_origin.begin());
    cout<<endl;
    delete dbs;
}

void test_grid(Labels& label_grid){

    //DBSCAN* dbs = new DBSCAN_Grid(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    DBSCAN* dbs = new DBSCAN_Grid(10000, 4);
    dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");

    cout<<"start execution of grid based DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_grid");

    Labels lbg = dbs->get_labels();
    label_grid.resize(lbg.size());
    std::copy(lbg.begin(), lbg.end(), label_grid.begin());
    cout<<endl;
    delete dbs;
}

/*
void test_reduced(Labels& label_reduced){
    
    //DBSCAN* dbs = new DBSCAN_Reduced(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    DBSCAN* dbs = new DBSCAN_Reduced(10000, 4);
    dbs->read_cluster_data(2, 5000, "../data/5times_s1.txt");

    cout<<"start execution of reduced grid based DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_reduced");

    Labels lbr = dbs->get_labels();
    label_reduced.resize(lbr.size());
    std::copy(lbr.begin(), lbr.end(), label_reduced.begin());
    cout<<endl;
    delete dbs;
}

void test_dfe(Labels& label_dfe){

    //DBSCAN* dbs = new DBSCAN_DFE(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    DBSCAN* dbs = new DBSCAN_DFE(10000, 4);
    dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");
    
    cout<<"start execution of dfe DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_dfe");

    Labels lbr = dbs->get_labels();
    label_dfe.resize(lbr.size());
    std::copy(lbr.begin(), lbr.end(), label_dfe.begin());
    cout<<endl;
    delete dbs;
}
*/

int main()
{
    
    Labels label_origin;
    Labels label_grid;
    //Labels label_reduced;
    //Labels label_dfe;

    test_original(label_origin);
    test_grid(label_grid);
    //test_reduced(label_reduced);
    //test_dfe(label_dfe);

    DBSCAN::cmp_result(label_origin, label_grid);
    //DBSCAN::cmp_result(label_grid, label_dfe);

    return 0;
}
