#include <iostream>
#include <fstream>
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

    int which_dim = 0;
    float basic_offset = 1200.0f;
    int times_offset = 1;

    int part = input_size / division;

    for(int i=0; i<division; i++){
        float offset = basic_offset * times_offset;
        int temp = i * part;
        for(int j=0; j<part; j++){
            int which = temp + j;
            cl_d(which, which_dim) += offset;
        }
        which_dim++;
        if(which_dim == dim){
            which_dim = 0;
            times_offset++;
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
