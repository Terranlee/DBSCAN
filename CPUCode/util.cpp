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
    start.resize(d);
    stop.resize(d);
}

void MultiIteration::set_max(const std::vector<int>& max){
    max_val.resize(max.size());
    std::copy(max.begin(), max.end(), max_val.begin());
    assert(dim = max_val.size());
}

void MultiIteration::set_start(const std::vector<int>& s){
    // point to the first cell in neighbour
    std::copy(s.begin(), s.end(), start.begin());
    for(unsigned int i=0; i<start.size(); i++)
        start[i] -= length;
    std::copy(s.begin(), s.end(), stop.begin());
    for(unsigned int i=0; i<stop.size(); i++)
        stop[i] += length;
}

void MultiIteration::test(){
    // currently do nothing here
    return;
}

/*
// the decode function is not possible using this hash function
void MultiIteration::decode(HashType hashKey, std::vector<int>& vec){
    // do not call vec.resize in this function
    // allocate the memory outside this function
    for(int i=dim-1; i>=1; i--){
        vec[i] = hashKey % max_val[i-1];
        hashKey = hashKey / max_val[i];
    }
    vec[0] = hashKey;
}
*/

void MultiIteration::next(){
    int end = dim - 1;
    if(start[end] == stop[end]){
        start[end] = stop[end] - 2 * length;
        for(int i=end-1; i>=0; i--){
            if(start[i] == stop[i])
                start[i] = stop[i] - 2 * length;
            else{
                start[i]++;
                break;
            }
        }
        return;
    }
    start[end]++;
}

HashType MultiIteration::get_hash(){
    // TODO: is it okay to use int as the hashKey?
    // maybe too short in high dimension dataset
    HashType hashKey = 0;
    for(int i=0; i<dim-1; i++){
        hashKey += start[i];
        hashKey *= max_val[i];
    }
    hashKey += start[dim-1];
    return hashKey;
}

HashType MultiIteration::hash(const std::vector<int>& vec){
    HashType hashKey = 0;
    for(int i=0; i<dim-1; i++){
        hashKey += vec[i];
        hashKey *= max_val[i];
    }
    hashKey += vec[dim-1];
    return hashKey;
}

const std::vector<int>& MultiIteration::get_vec(){
    return start;
}

HashType MultiIteration::get_next(){
    next();
    return get_hash();
}
