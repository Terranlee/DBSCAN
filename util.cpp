#include "util.h"

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

int UnionFind::get_size(int i){
    return union_find[i].second;
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

void UnionFind::print_union(){
    // this function should be called after merge_clusters
    cout<<"-----------print union find information-----------"<<endl;
    cout<<"union_find count:"<<count<<endl;
    for(unsigned int i=0; i<union_find.size(); i++){
        cout<<i<<", "<<union_find[i].first<<", "<<union_find[i].second<<endl;
    }
    cout<<"-------------------------------------"<<endl;
}

void UnionFind::test(){

}

