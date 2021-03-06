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
            fout<<"\t"<<int(temp);
        }
        fout<<endl;
    }

    fout.close();
    fin.close();
}

int main(){
    //to_cuhk("household.data", 2049280, 7);
    //to_cuhk("clustered_1M_3D", 1000303, 3);
	to_cuhk("clustered_5M_5D.offset.data", 5000367, 5);
	to_cuhk("/home/trli/download/PAMAP2_Dataset/answer.txt.data", 3850505, 4);
    return 0;
}
