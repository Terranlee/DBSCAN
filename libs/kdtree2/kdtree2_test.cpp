//
// A demonstration of using the KDTREE2 C++ routines, and timing.
// This file is in the public domain.
//

#include "kdtree2.hpp"

#include <boost/multi_array.hpp>
#include <boost/random.hpp>

using namespace boost; 


static minstd_rand generator(42u); 
static uniform_real<> uni_dist(0,1); 
variate_generator<minstd_rand&,uniform_real<> > uni(generator,uni_dist); 

float random_variate() {
  // between [0,1)
  return(uni()); 
}

//
// define, for convenience a 2d array of floats. 
//
typedef multi_array<float,2> array2dfloat;


#include <ctime>

float time_a_search(kdtree2* tree, int nn, int nsearch) {
  int dim = tree->dim;
  vector<float> query(dim);
  kdtree2_result_vector result; 

  clock_t t0, t1; 

  t0 = clock();

  for (int i=0; i<nsearch;i++) {
      for (int j=0; j<dim; j++) query[j] = random_variate(); 
      tree->n_nearest(query,nn,result);
  }

  t1 = clock(); 

  return(static_cast<float> 
	 (static_cast<double> (t1-t0) / static_cast<double> (CLOCKS_PER_SEC) ));
}

void time_random_searches(kdtree2* tree, int nn) {
  // emit the number of searches per second.
  int nsearch;

  nsearch = 50;
  for(;;) {
    float t = time_a_search(tree,nn,nsearch); 
    if (t < 1.0) {
      nsearch *= 5; 
      continue; 
    } else {
      float sps = float(nsearch) / t ;
      cout << "C++ impl, for nn=" << nn << " searches/sec = " << sps << "\n";
      return;
    }
  }
}

int main() {
  array2dfloat realdata; 

  // notice it is in C-standard layout. 
  kdtree2* tree;
  kdtree2_result_vector res; 

  int length = 5;
  int N = length * length;
  int dim = 2;

  realdata.resize(extents[N][dim]); 
  
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
    vector<float> query(dim); 
    kdtree2_result_vector result;
    
    query[0] = 2.5f;
    query[1] = 2.5f;

    tree->r_nearest(query, 1, result);
    
    for(int i=0; i<result.size(); i++){
      int id = result[i].idx;
      for(int j=0; j<dim; j++)
        cout<<realdata[id][j]<<",";
      cout<<"      "<<result[id].dis;
      cout<<endl;
    }
  }

  return 0;
}

