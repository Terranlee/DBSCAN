#include <iostream>
#include <unordered_map>
#include <fstream>

using namespace std;

int main(){
    unordered_map<string, int> mapping;
    ifstream fin("household.data");
    int counter = 0;
    string content;
    while(getline(fin, content)){
        unordered_map<string, int>::iterator got = mapping.find(content);
        if(got == mapping.end()){
            mapping.insert(make_pair(content, 1));
        }
        else{
            counter++;
        }
    }
    fin.close();
    cout<<counter<<endl;

    ofstream fout("deduplicate.data");
    for(unordered_map<string, int>::const_iterator iter = mapping.begin(); iter != mapping.end(); ++iter){
        fout<<iter->first<<endl;
    }
    fout.close();
    return 0;
}
