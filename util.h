#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <iostream>

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
    int get_size(int i);
    void make_union(int p, int q);
    void print_union();
    void test();
};


#endif
