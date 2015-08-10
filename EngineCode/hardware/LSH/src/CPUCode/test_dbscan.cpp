#include "dbscan_matrix.h"
#include "dbscan_grid.h"
#include "dbscan_reduced.h"
#include "dbscan_lsh.h"
#include "dbscan_kd.h"

// the dfe version of DBSCAN does not support dimension > 3
// checkout to branch main for code that are able to run on DFE
/*
#ifdef DESIGN_NAME
    #include "dbscan_dfe.h"
#endif
*/

#include <algorithm>

using namespace clustering;

void test_original(Labels& label_origin){

    //DBSCAN* dbs = new DBSCAN_Matrix(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    //DBSCAN* dbs = new DBSCAN_Matrix(10000, 4);
    //dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");

    // high dimension data
    DBSCAN* dbs = new DBSCAN_Matrix(200, 64);
    dbs->read_cluster_data(5, 10000, "../data/dim5.txt");

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

void test_kd(Labels& label_kd){
    //DBSCAN* dbs = new DBSCAN_KD(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    DBSCAN* dbs = new DBSCAN_KD(10000, 4);
    dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");
    
    // high dimension data
    //DBSCAN* dbs = new DBSCAN_KD(5000.0, 100);
    //dbs->read_cluster_data(7, 1906698, "../data/deduplicate.data");

    cout<<"start execution of distance kdtree DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_kd");

    Labels lbo = dbs->get_labels();
    label_kd.resize(lbo.size());
    std::copy(lbo.begin(), lbo.end(), label_kd.begin());
    cout<<endl;
    delete dbs;

}

void test_grid(Labels& label_grid){

    //DBSCAN* dbs = new DBSCAN_Grid(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    DBSCAN* dbs = new DBSCAN_Grid(10000, 4);
    dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");

    // high dimension
    //DBSCAN* dbs = new DBSCAN_Grid(1250.0, 50);
    //dbs->read_cluster_data(3, 500000, "../data/clustered_2M_3D.data");

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


void test_reduced(Labels& label_reduced){
    
    //DBSCAN* dbs = new DBSCAN_Reduced(20000, 4);   // the papameter for s1.txt
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    DBSCAN* dbs = new DBSCAN_Reduced(10000, 4);
    dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");

    // high dimension data
    // DBSCAN* dbs = new DBSCAN_Reduced(200, 8);
    //  dbs->read_cluster_data(5, 5000, "../data/dim5.txt");

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

void test_lsh(Labels& label_lsh){

    //DBSCAN* dbs = new DBSCAN_LSH(20000, 4);
    //dbs->read_cluster_data(2, 5000, "../data/s1.txt");

    //DBSCAN* dbs = new DBSCAN_LSH(10000, 4);
    //dbs->read_cluster_data(2, 25000, "../data/5times_s1.txt");

    // high dimension data
    DBSCAN* dbs = new DBSCAN_LSH(5000.0, 100);
    dbs->read_cluster_data(7, 2049280, "../data/household.data");

    //DBSCAN* dbs = new DBSCAN_LSH(5000.0, 100);
    //dbs->read_cluster_data(3, 1000000, "../data/clustered_2M_3D.data");

    cout<<"start execution of lsh based DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs->fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;

    dbs->reshape_labels();
    dbs->output_result("output_lsh");
    Labels lbr = dbs->get_labels();
    label_lsh.resize(lbr.size());
    std::copy(lbr.begin(), lbr.end(), label_lsh.begin());
    cout<<endl;
    delete dbs;
}

/*
// test dbscan on dfe only when you have maxeler environment
#ifdef DESIGN_NAME
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
#endif
*/

int main()
{
    
    //Labels label_origin;
    Labels label_kd;
    Labels label_grid;
    //Labels label_reduced;
    //Labels label_lsh;

    //test_original(label_origin);
    test_kd(label_kd);
    test_grid(label_grid);
    //test_reduced(label_reduced);
    //test_lsh(label_lsh);
    DBSCAN::cmp_result(label_grid, label_kd);

    //DBSCAN::get_max(label_grid);

    //DBSCAN::cmp_result(label_lsh, label_grid);
    //DBSCAN::cmp_result(label_origin, label_grid);
    //DBSCAN::cmp_result(label_origin, label_kd);
    //DBSCAN::cmp_result(label_origin, label_reduced);
    
    return 0;
}
