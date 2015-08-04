#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>

using namespace std;

void randomize(vector<int>& intvec){
    srand(unsigned(time(NULL)));
    unsigned int sz = intvec.size();
    for(unsigned int i=0; i<sz; i++){
        int index = rand() % (sz - i) + i;
        int temp = intvec[i];
        intvec[i] = intvec[index];
        intvec[index] = temp;
    }
}

int main(){

    vector<int> intvec(10);
    for(int i=0; i<10; i++)
        intvec[i] = i;

    randomize(intvec);

    for(int i=0; i<10; i++)
        cout<<intvec[i]<<" ";
    cout<<endl;
    
    return 0;
}