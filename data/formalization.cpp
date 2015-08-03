#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

using namespace std;
using namespace boost::numeric;

const float max_all = 100000.0f;
const float min_all = 0.0f;

void formalize_generate_data(const int dim_in, const int line, const string filename){
    ifstream fin(filename.data());
    ublas::matrix<float> cl_d(line, dim_in);
    for(int i=0; i<line; i++){
        for(int j=0; j<dim_in; j++)
            fin>>cl_d(i, j);
    }
    fin.close();

    vector<float> min_val(dim_in);
    vector<float> max_val(dim_in);
    for(int i=0; i<dim_in; i++){
        min_val[i] = std::numeric_limits<float>::max();
        max_val[i] = std::numeric_limits<float>::min();
    }

    for(int i=0; i<cl_d.size1(); i++){
        for(int j=0; j<dim_in; j++){
            if(cl_d(i, j) > max_val[j])
                max_val[j] = cl_d(i, j);
            if(cl_d(i, j) < min_val[j])
                min_val[j] = cl_d(i, j);
        }
    }

    vector<float> diff(dim_in);
    for(int i=0; i<dim_in; i++)
        diff[i] = max_val[i] - min_val[i];
    for(int i=0; i<dim_in; i++)
        diff[i] = (max_all - min_all) / diff[i];

    string output = filename + ".data";
    ofstream fout(output.data());
    for(int i=0; i<cl_d.size1(); i++){
        for(int j=0; j<cl_d.size2(); j++)
            fout<<cl_d(i, j)<<" ";
        fout<<endl;
    }
    fout.close();
}

// formalization of UCI dataset Household
void formalize_household(){
    const string input = "household_power_consumption.txt";
    const string output = "household.data";
    const string sep = ";";
    const string invalid = "?";
    const int dim_in = 9;
    const int dim_out = 7;
    
    ifstream fin(input.data());
    string content;
    getline(fin, content);   // title
    int counter = 0;

    vector<vector<float> > matrix;
    while(getline(fin, content)){
        size_t index = content.find(invalid);
        if(index == string::npos){
            counter++;
            size_t id = content.find(sep);
            id = content.find(sep, id + 1);
            stringstream sstrm(content.substr(id));

            vector<float> vec(dim_out);
            char temp;
            for(int i=0; i<dim_out; i++)
                sstrm >> temp >> vec[i];
            matrix.push_back(vec);
        }
    }
    fin.close();
    cout<<counter<<" valid data"<<endl;

    vector<float> min_val(dim_out);
    vector<float> max_val(dim_out);
    for(int i=0; i<dim_out; i++){
        min_val[i] = std::numeric_limits<float>::max();
        max_val[i] = std::numeric_limits<float>::min();
    }

    for(int i=0; i<matrix.size(); i++){
        for(int j=0; j<dim_out; j++){
            if(matrix[i][j] > max_val[j])
                max_val[j] = matrix[i][j];
            if(matrix[i][j] < min_val[j])
                min_val[j] = matrix[i][j];
        }
    }

    cout<<"max data:"<<endl;
    for(int i=0; i<dim_out; i++)
        cout<<max_val[i]<<" ";
    cout<<endl;

    cout<<"min data:"<<endl;
    for(int i=0; i<dim_out; i++)
        cout<<min_val[i]<<" ";
    cout<<endl;

    vector<float> diff(dim_out);
    for(int i=0; i<dim_out; i++)
        diff[i] = max_val[i] - min_val[i];
    for(int i=0; i<dim_out; i++)
        diff[i] = (max_all - min_all) / diff[i];

    ofstream fout(output.data());
    for(int i=0; i<matrix.size(); i++){
        for(int j=0; j<dim_out; j++){
            float num = (matrix[i][j] - min_val[j]) * diff[j];
            fout<<num<<" ";
        }
        fout<<endl;
    }
    fout.close();
}

int main(){
    formalize_household();
    return 0;
}