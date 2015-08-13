#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int cmp_vector(const vector<int>& vec1, const vector<int>& vec2){
    int counter = 0;
    int iter1 = 0;
    int iter2 = 0;

    while(iter1 < vec1.size() && iter2 < vec2.size()){
        if(vec1[iter1] == vec2[iter2]){
            counter++;
            iter1++;
            iter2++;
        }
        else if(vec1[iter1] < vec2[iter2])
            iter1++;
        else
            iter2++;
    }
    return counter;
}

int cmp_result(const string file1, const string file2, int dim){
    // file1 is the std answer
    // file2 is the approximate answer 
    ifstream fin1(file1.data());
    ifstream fin2(file2.data());

    int size1, size2;
    fin1>>size1;
    fin2>>size2;

    vector<int> ans1(size1);
    vector<int> ans2(size2);

    float temp;
    for(int i=0; i<size1; i++){
        fin1>>ans1[i];
        for(int j=0; j<dim; j++)
            fin1>>temp;
    }

    for(int i=0; i<size2; i++){
        fin2>>ans2[i];
        for(int j=0; j<dim; j++)
            fin2>>temp;
    }
    fin1.close();
    fin2.close();

    int result = cmp_vector(ans1, ans2);

    float ans = float(result) / float(ans1.size());
    cout<<"cluster accuracy : "<<ans<<endl;
    return result;
}

int main(){

    return 0;
}