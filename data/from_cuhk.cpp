#include <iostream>
#include <fstream>

using namespace std;

void from_cuhk(const string filename, int input_size, int dimension){
    ifstream fin(filename.data());
    string output_file = filename + ".data";
    ofstream fout(output_file.data());

    string title;
    getline(fin, title);

    int counter;
    float data;
    for(int i=0; i<input_size; i++){
        fin>>counter;
        for(int j=0; j<dimension; j++){
            fin>>data;
            fout<<data<<" ";
        }
        fout<<endl;
    }

    fout.close();
    fin.close();
}

int main(){

    return 0;
}
