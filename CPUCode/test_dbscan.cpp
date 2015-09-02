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
#include <cstdlib>
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

void test_lsh(int iter, float eps, int minPts, int dim, int length, std::string filename){
    // high dimension data
    DBSCAN_LSH dbs(eps, minPts, iter);
    dbs.read_cluster_data(dim, length, filename);
	
	// set the debug value
	//dbs.debug_value = debug_value;

    cout<<"start execution of lsh based DBSCAN"<<endl;
    float begin = DBSCAN::get_clock();
    dbs.fit();
    float end = DBSCAN::get_clock();
    cout<<"time is : "<<end - begin<<endl;
	
    dbs.reshape_labels();
	Labels lbr = dbs.get_labels();
	DBSCAN::get_max(lbr);
	//dbs->output_same_result_cuhk();

    cout<<endl;
}

int main(int argc, char** argv)
{

	if(argc != 7 && argc != 8){
		cout<<"./test_dbscan iter eps minPts dim length filename [debug_value]"<<endl;
		exit(1);
	}

    int iter = atoi(argv[1]);
	float eps = atof(argv[2]);
	int minPts = atoi(argv[3]);
	int dim = atoi(argv[4]);
	int length = atoi(argv[5]);
	std::string filename = std::string(argv[6]);
/*	
	int debug_value = 300;
	if(argc == 8)
		debug_value = atoi(argv[7]);
*/
    //Labels label_kd;
    //Labels label_grid;
    //Labels label_lsh;

    //test_kd(label_kd);
    //test_grid(label_grid);
    test_lsh(iter, eps, minPts, dim, length, filename);

    return 0;
}
