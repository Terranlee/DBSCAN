#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric;
using namespace std;
typedef ublas::matrix<float> ClusteringData;

void add_offset(const string filename, int dim, int input_size, int division){
    ClusteringData cl_d = ClusteringData(input_size, dim);

    ifstream fin(filename.data());
    for(int i=0; i<input_size; i++)
        for(int j=0; j<dim; j++)
            fin>>cl_d(i, j);
    fin.close();

    srand(unsigned(time(NULL)));

    int max_times = 5;
    float basic_offset = 1600.0f;
    vector<float> offset(dim);

    int part = input_size / division;
    for(int i=0; i<division; i++){
        for(int k=0; k<dim; k++){
            int rnd = rand() % max_times;
            offset[k] = basic_offset * rnd;
        }

        int temp = i * part;
        for(int j=0; j<part; j++){
            int which = temp + j;
            for(int k=0; k<dim; k++)
                cl_d(which, k) += offset[k];
        }

    }

    string output_file = filename + ".offset";
    ofstream fout(output_file.data());
    for(int i=0; i<cl_d.size1(); i++){
        for(int j=0; j<cl_d.size2(); j++)
            fout<<cl_d(i, j)<<" ";
        fout<<endl;
    }
    fout.close();
}

int main(){

    add_offset("clustered_1M_3D", 3, 1000303, 20);
    return 0;
}
