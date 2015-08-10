//
// A demonstration of using the KDTREE2 C++ routines, and timing.
// This file is in the public domain.
//

#include "kdtree2.hpp"

#include <boost/multi_array.hpp>
#include <boost/random.hpp>

//
// define, for convenience a 2d array of floats. 
//
typedef boost::multi_array<float,2> array2dfloat;


#include <ctime>

int main() {
  array2dfloat realdata; 

  // notice it is in C-standard layout. 
  kdtree2* tree;
  kdtree2_result_vector res; 

  int length = 5;
  int N = length * length;
  int dim = 2;

  realdata.resize(boost::extents[N][dim]); 
  
  for(int i=0; i<length; i++)  
    for(int j=0; j<length; j++){
        realdata[i*length+j][0] = i + 0.5;
        realdata[i*length+j][1] = j + 0.5;
    }
  
  for(int i=0; i<length; i++)  
    for(int j=0; j<length; j++){
        cout<<realdata[i*length+j][0]<<" "<<realdata[i*length+j][1]<<endl;
    }
  
  tree = new kdtree2(realdata,true);
  tree->sort_results = true;
  cout << "Tree created, now testing against brute force..."<<endl; 
  {
    std::vector<float> query(dim); 
    kdtree2_result_vector result;
    
    query[0] = 2.5f;
    query[1] = 2.5f;

    tree->r_nearest(query, 1.9, result);
    
    for(unsigned int i=0; i<result.size(); i++){
      int id = result[i].idx;
      for(int j=0; j<dim; j++)
        cout<<realdata[id][j]<<",";
      cout<<"      "<<result[i].dis;
      cout<<endl;
    }
  }

  return 0;
}

