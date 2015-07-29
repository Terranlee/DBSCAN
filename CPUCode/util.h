#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>

using std::cout;
using std::endl;
using namespace boost::numeric;

// use union_find to record which cells belong to the same cluster
class UnionFind{
private:
    // first is the parent pointer, second is the size of this union_find node
    std::vector<std::pair<int, int> > union_find;
    int count;
public:
    UnionFind();
    UnionFind(int size);
    void init(int size);

    // two main functions during the search in neighbour
    int find(int i);
    void make_union(int p, int q);

    void print_union() const;
    int get_size(int i) const;
    int get_count() const;
    void test();
};

// HashType is the datatype used in high dimension grid
// use uint64_t, not uint32_t, to avoid overflow
typedef int64_t HashType;

// use this to do a multi-dimension iteration when searching in the neighbour
class MultiIteration{
private:
    // dim means dimension, length means the number of iterations in each size of a direction
    // max_val is the max size of the grid
    // these three members are set during a DBSCAN execution
    int dim;
    int length;
    std::vector<int> max_val;
    int counter;

    // this member is set during a neighbour iteration
    HashType value;
    std::vector<int> iter;

public:
    MultiIteration();
    MultiIteration(unsigned int d);
    void set_dimension(unsigned int d);
    void set_max(const std::vector<int>& max);
    void set_start(HashType val);
    void test();

    // next() : iterate to the next neighbour, return its key in hash_grid
    // get()  : get the current hash value
    // hash() : return the hash code
    // get_counter() : get the number of iteration that is needed in this dimension
    HashType next();
    HashType get() const;
    HashType hash(const std::vector<int>& vec) const;
    int get_counter() const;
};

// the defination of LSH has moved to dbscan_lsh.h

#endif
