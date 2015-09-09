#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

vector<vector<int> > correct;
int sum_size = 0;

void load(string dir, int counter, int dim){
    correct.clear();
    vector<int> empty;
    correct.push_back(empty);

    for(int i=1; i<=counter; i++){
		stringstream sstrm;
		sstrm << i;
		string which;
		sstrm >> which;
		string filename = dir + "Cluster_" + which;
        ifstream fin(filename.data());
        int temp, count;
        fin>>count;
		cout<<"loading : "<<filename<<" : "<<count<<endl;
        vector<int> intvec(count);
        for(int j=0; j<count; j++){
            fin>>intvec[j];
            for(int k=0; k<dim; k++)
                fin>>temp;
        }
        sum_size += intvec.size();
        correct.push_back(intvec);
        fin.close();
    }
}

int cmp_vector(const vector<int>& std, const vector<int>& my){
    int counter = 0;
    int iter1, iter2;
    iter1 = iter2 = 0;
    while(iter1 < std.size() && iter2 < my.size()){
        if(std[iter1] == my[iter2]){
            counter++;
            iter1++;
            iter2++;
        }
        else if(std[iter1] < my[iter2]){
            iter1++;
        }
        else{
            iter2++;
        }
    }
    return counter;
}

void check(string dir, int counter, int dim){
    int corr_counter = 0;
    for(int i=1; i<=counter; i++){
		stringstream sstrm;
		sstrm << i;
		string which;
		sstrm >> which;
		string filename = dir + "Cluster_" + which;
        ifstream fin(filename.data());
        int temp, count;
        fin>>count;
		cout<<"loading : "<<filename<<" : "<<count<<endl;
        vector<int> intvec(count);
        for(int j=0; j<count; j++){
            fin>>intvec[j];
        }
        fin.close();
        int sz = intvec.size();
        int sz_max = int(float(sz) * 2);

        int adder = 0;
		int belong = 0;
		int belong_id = -1;
        for(int iter=0; iter<correct.size(); iter++){
            if( correct[iter].size() >= sz && correct[iter].size() <= sz_max){
                int ans = cmp_vector(correct[iter], intvec);
                if(ans > adder){
                    adder = ans;
					belong = correct[iter].size();
					belong_id = iter;
				}
            }
        }
        corr_counter += adder;
		cout<<adder<<"\t"<<"Cluster_"<<belong_id<<" : "<<belong<<endl;
    }

	cout<<"----------------------------------------------"<<endl;
    cout<<corr_counter<<" : "<<sum_size<<endl;
    float rate = float(corr_counter) / float(sum_size);
    cout<<"rate : "<<rate<<endl;
}

int main(int argc, char** argv){
	load("/home/trli/CUHK/Clustering_Result/OurExact/", 53, 9);
	string str_cnt = string(argv[1]);
	int cnt;
	stringstream sstrm;
	sstrm << str_cnt;
	sstrm >> cnt;
	check("./Clustering_Result_real/", cnt, 9);
    return 0;
}
