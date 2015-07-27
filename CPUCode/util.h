#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

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
    int find(int i);
    int get_size(int i) const;
    int get_count() const;
    void make_union(int p, int q);
    void print_union() const;
    void test();
};

// HashType is the datatype used in high dimension grid
// use uint64_t, not uint32_t, to avoid overflow
typedef uint64_t HashType;

// use this to do a multi-dimension iteration when searching in the neighbour
class MultiIteration{
private:
    // dim means dimension, length means the number of iterations in each size of a direction
    // max_val is the max size of the grid
    // these three members are set during a DBSCAN execution
    int dim;
    int length;
    std::vector<int> max_val;

    // the following members are set during each iteration
    // start[0] represents the outer loop, start[start.size()-1] represents the inner loop
    std::vector<int> start;
    std::vector<int> stop;

public:
    MultiIteration();
    MultiIteration(unsigned int d);
    void set_dimension(unsigned int d);
    void set_max(const std::vector<int>& max);
    void set_start(const std::vector<int>& s);
    void test();

    // the decode function is not possible using current hash function
    // avoid using this function
    //void decode(HashType hashKey, std::vector<int>& vec);

    // next() : iterate to the next neighbour
    // get_hash() : get the hashKey of the current cell
    // hash() : return the hash code
    // get_vec() : get the vector representation of the current cell
    // get_next() : the combination of both next() and get()
    void next();
    HashType get_hash();
    HashType hash(const std::vector<int>& vec);
    const std::vector<int>& get_vec();
    HashType get_next();
};

#endif
