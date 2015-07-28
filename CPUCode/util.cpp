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
    std::copy(max.begin(), max.end(), max_val.begin());
    assert(dim = max_val.size());
}

void MultiIteration::set_start(HashType val){
    // this function get the center of neighbours
    // and then set to the beginning cell of the neighbours
    value = val;
    for(int i=0; i<dim; i++)
        iter[i] = -1 * length;
    for(int i=0; i<dim-1; i++)
        value -= length * max_val[i];
    value -= length;
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
    unsigned int end = dim - 1;
    if(iter[end] == length){
        iter[end] = -1 * length;
        value -= length * 2;
        for(int i=end-1; i>=0; i--){
            if(iter[i] == length){
                iter[i] = -1 * length;
                value -= max_val[i] * length * 2;
            }
            else{
                iter[i]++;
                value += max_val[i];
                break;
            }
        }
        return value;
    }
    iter[end]++;
    value += 1;
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

/************************************************************************/
// The following functions are for the locality sensitive hashing method
LSH::LSH(){}
LSH::LSH(int din, int dout){
    set_dimensions(din, dout);
}

void LSH::set_dimensions(int din, int dout){
    // the function matrix has dout lines, and din rows
    // the line of the matrix can be used to do projection
    m_hash = Functions(dout, din);
}

void LSH::generate(){
    // may be changed to srand((unsigned) time(NULL))
    srand(0);
    for(unsigned int i=0; i<m_hash.size1(); i++)
        for(unsigned int j=0; j<m_hash.size2(); j++)
            m_hash(i, j) = MULTIPLIER * float(rand()) / float(RAND_MAX);
}

unsigned int LSH::get_out_dimension() const{
    return m_hash.size1();
}

void LSH::test(){
    for(unsigned int i=0; i<m_hash.size1(); i++){
        for(unsigned int j=0; j<m_hash.size2(); j++)
            cout<<m_hash(i, j)<<"\t";
        cout<<endl;
    }
}
