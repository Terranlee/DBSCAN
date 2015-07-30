#include "util.h"

/************************************************************************/
// The following functions are for the union find data structure
UnionFind::UnionFind(){}

UnionFind::UnionFind(int size){
    init(size);
}

void UnionFind::init(int size){
    union_find.resize(size);
    for(int i=0; i<size; i++){
        union_find[i].first = i;
        union_find[i].second = 1;
    }
    count = size;
}

int UnionFind::find(int i){
    while(i != union_find[i].first){
        union_find[i].first = union_find[ union_find[i].first ].first;
        i = union_find[i].first;
    }
    return i;
}

int UnionFind::get_size(int i) const{
    return union_find[i].second;
}

int UnionFind::get_count() const{
    return count;
}

void UnionFind::make_union(int p, int q){
    int i = find(p);
    int j = find(q);
    if(i == j)    return;
    if(union_find[i].second < union_find[j].second){
        union_find[i].first = j;
        union_find[i].second += union_find[j].second;
    }
    else{
        union_find[j].first = i;
        union_find[j].second += union_find[i].second;
    }
    count--;
}

void UnionFind::print_union() const{
    // this function should be called after merge_clusters
    cout<<"-----------print union find information-----------"<<endl;
    cout<<"union_find count:"<<count<<endl;
    for(unsigned int i=0; i<union_find.size(); i++){
        cout<<i<<", "<<union_find[i].first<<", "<<union_find[i].second<<endl;
    }
    cout<<"-------------------------------------"<<endl;
}

void UnionFind::test(){
    // currently do nothing here
    return;
}

/************************************************************************/
// The following functions are for the multi-iteration data structure
MultiIteration::MultiIteration(){}
MultiIteration::MultiIteration(unsigned int d){
    set_dimension(d);
}

void MultiIteration::set_dimension(unsigned int d){
    dim = d;
    // do #length iterations on each side of one dimension
    length = std::ceil( std::sqrt(float(d)) );
    iter.resize(dim);
    int width = length * 2 + 1;
    counter = int( pow( float(width), float(dim)) ) - 1;
    // for debug
    cout<<"dim:"<<dim<<" length:"<<length<<" counter:"<<counter<<endl;
}

void MultiIteration::set_max(const std::vector<int>& max){
    max_val.resize(max.size());
    diff.resize(max.size());
    std::copy(max.begin(), max.end(), max_val.begin());

    diff[diff.size()-1] = 1;
    for(int i=diff.size()-2; i>=0; i--)
        diff[i] = diff[i+1] * max[i];
}

void MultiIteration::set_start(HashType val){
    // this function get the center of neighbours
    // and then set to the beginning cell of the neighbours
    value = val;
    for(int i=0; i<dim; i++)
        iter[i] = -1 * length;
    for(int i=0; i<dim; i++)
        value -= length * diff[i];
    // now value is the key to the beginning cell of neighbours
}

void MultiIteration::test(){
    // currently do nothing here
    return;
}

HashType MultiIteration::get() const{
    return value;
}

HashType MultiIteration::next(){
    for(int i=dim-1; i>=0; i--){
        if(iter[i] == length){
            iter[i] = -1 * length;
            value -= diff[i] * length * 2;
        }
        else{
            iter[i]++;
            value += diff[i];
            break;
        }
    }
    return value;
}

HashType MultiIteration::hash(const std::vector<int>& vec) const{
    HashType hashKey = 0;
    for(int i=0; i<dim-1; i++){
        hashKey += vec[i];
        hashKey *= max_val[i];
    }
    hashKey += vec[dim-1];
    return hashKey;
}

int MultiIteration::get_counter() const{
    return counter;
}


