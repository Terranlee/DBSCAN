#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

float max_val = 20.0;

void gen_data(const string filename, int cnt){
    ofstream fout(filename.data());
    for(int i=0; i<cnt; i++){
        for(int j=0; j<2; j++){
            float data = float(rand()) / float(RAND_MAX) * max_val;
            fout<<data<<" ";
        }
        fout<<endl;
    }
    fout.close();
}

int main(){
    srand(unsigned(time(NULL)));
    gen_data("reduced_precision.data", 400);
    return 0;
}
