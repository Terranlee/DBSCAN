#include <iostream>
#include <fstream>

using namespace std;

// change the data format to CUHK SIGMOD15 DBSCAN
void to_cuhk(const string filename, int input_size, int dimension){
    ifstream fin(filename.data());
    string output_file = filename + ".ds";
    ofstream fout(output_file.data());

    fout<<input_size<<"\t"<<dimension<<endl;
    float temp;
    for(int i=0; i<input_size; i++){
        fout<<i+1;
        for(int j=0; j<dimension; j++){
            fin>>temp;
            fout<<"\t"<<temp;
        }
        fout<<endl;
    }

    fout.close();
    fin.close();
}

int main(){
    to_cuhk("clustered_1M_3D", 1000303, 3);
    return 0;
}
