#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <boost/numeric/ublas/matrix.hpp>

using namespace std;
using boost::numeric::ublas::matrix;


void read_cluster_data(matrix<double>& cl_d, size_t elements_num, size_t features_num, string filename){
    ifstream fin(filename.data());
    for(size_t i=0; i<elements_num; i++)
        for(size_t j=0; j<features_num; j++)
            fin>>cl_d(i, j);
    fin.close();
}

void multiply_data(const matrix<double>& cl_d, unsigned int times_multiply, double range, string filename){
    stringstream stream;
    string str_multiply;

    stream << times_multiply;
    stream >> str_multiply;
    string out_file = str_multiply + "times_" + filename;

    ofstream fout(out_file.data());
    for(unsigned int i=0; i<times_multiply; i++){
        for(unsigned int j=0; j<cl_d.size1(); j++){
            for(unsigned int k=0; k<cl_d.size2(); k++){
                double d = double(rand()) / double(RAND_MAX) * range;
                if(rand() % 2 == 0)
                    fout<<cl_d(j,k) + d<<" ";
                else
                    fout<<cl_d(j,k) - d<<" ";
            }
            fout<<endl;
        }
    }
    fout.close();
}

int main(){
    srand(unsigned(time(NULL)));
    int elements_num = 5000;
    int features_num = 2;
    unsigned int times_multiply = 5;
    double range = 8000.0;

    string filename = "s1.txt";

    matrix<double> cl_d = matrix<double>(elements_num, features_num);
    read_cluster_data(cl_d, elements_num, features_num, filename);
    multiply_data(cl_d, times_multiply, range, filename);
    
    return 0;
}