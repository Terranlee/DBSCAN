#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>
#include <algorithm>
#include <functional>
#include <map>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <vector>
#include <sstream>

#include "dbscan.h"

namespace clustering
{
    void DBSCAN::read_cluster_data(size_t features_num, size_t elements_num, std::string filename){
        cl_d = ClusterData(elements_num, features_num);
        std::ifstream fin(filename.data());
		std::string header;
		getline(fin, header);
		int point_id;
        for(size_t i=0; i<elements_num; i++){
			fin>>point_id;
            for(size_t j=0; j<features_num; j++)
                fin>>cl_d(i, j);
		}
        fin.close();
        /*
        for(size_t i=0; i<10; i++){
            for(size_t j=0; j<features_num; j++)
                cout<<cl_d(i, j)<<" ";
            cout<<endl;
        }
        */
    }

    void DBSCAN::gen_cluster_data( size_t features_num, size_t elements_num ){
        cl_d = ClusterData(elements_num, features_num);
        for (size_t i = 0; i < elements_num; ++i){
            for (size_t j = 0; j < features_num; ++j)
                cl_d(i, j) = (-1.0 + rand() * (2.0) / RAND_MAX);
        }
    }

    void DBSCAN::cmp_result(const Labels& a, const Labels& b){
        /*
        int cnt_right = 0;
        assert(a.size() == b.size());

        for(unsigned int i=0; i<a.size(); i++)
            if(a[i] == b[i])
                cnt_right++;
        float rate = (float)cnt_right / (float)a.size();
        cout<<"similarity: "<<rate<<endl;
        */
        get_max(a);
        get_max(b);
    }

    void DBSCAN::get_max(const Labels& a){
        std::map<int, int> mapping;
        for(unsigned int i=0; i<a.size(); i++){
            std::map<int, int>::iterator got = mapping.find(a[i]);
            if(got == mapping.end())
                mapping.insert(std::make_pair(a[i], 1));
            else
                got->second++;
        }

        std::vector<int> max_size(mapping.size());
        int index = 0;
        for(std::map<int, int>::const_iterator iter = mapping.begin(); iter != mapping.end(); ++iter){
            if(iter->first == -1)
                max_size[index++] = -1 * iter->second;
            else
                max_size[index++] = iter->second;
        }
        sort(max_size.begin(), max_size.end(), std::greater<int>());
        int sz = std::min(50, (int)max_size.size());
        cout<<endl;
        for(int i=0; i<sz; i++)
            cout<<"size : "<<max_size[i]<<endl;
    }
	
	void DBSCAN::output_same_result_cuhk(){
		// output the clustering result just like the cuhk algorithm does
		std::unordered_map<int, std::vector<int> > final_result;
		for(unsigned int i=0; i<m_labels.size(); i++){
			std::unordered_map<int, std::vector<int> >::iterator got = final_result.find(m_labels[i]);
			if(got == final_result.end()){
				std::vector<int> intvec;
				intvec.push_back(i);
				final_result.insert(std::make_pair(m_labels[i], intvec));
			}
			else
				got->second.push_back(i);
		}
		/*	
		for(std::unordered_map<int, std::vector<int> >::const_iterator iter = final_result.begin(); iter != final_result.end(); ++iter){
			cout<<"size : "<<iter->second.size()<<endl;
		}
		*/

		const std::string result_root = "Clustering_Result_real/Cluster_";
		for(std::unordered_map<int, std::vector<int> >::const_iterator iter = final_result.begin(); iter != final_result.end(); ++iter){
			int label = iter->first;
			std::string filename = "";
			if(label == -1)
				filename = result_root + "noise";
			else{
				std::stringstream sstrm;
				sstrm << (label + 1);
				sstrm >> filename;
				filename = result_root + filename;
			}

			std::ofstream fout(filename.data());
			fout<<iter->second.size()<<endl;
			for(unsigned int i=0; i<iter->second.size(); i++){
				int which = iter->second[i];
				// here we output (which + 1) because we need to compare with the cuhk result
				fout<<(which + 1)<<endl;
				/*
				for(unsigned int j=0; j<cl_d.size2(); j++){
					fout<<"\t"<<cl_d(which, j);
				}
				*/
				//fout<<endl;
			}
			fout.close();
		}
	}

    float DBSCAN::get_clock(){
        clock_t t = clock();
        return float(t) / float(CLOCKS_PER_SEC);
    }

    std::ostream& operator<<(std::ostream& o, DBSCAN & d)
    {
		// icpc 12.1.4 does not support auto
		Labels lb = d.get_labels();
		for(unsigned int i=0; i<lb.size(); i++)
			o << lb[i] << " ";
        //for(const auto & l : d.get_labels())
        //    o << l << " ";
        o << endl;
        return o;
    }

    DBSCAN::DBSCAN(){    }

    DBSCAN::DBSCAN(float eps, size_t min_elems)
    :m_min_elems( min_elems ){
        // given eps, convert to eps_sqr for convenience
        m_eps_sqr = eps * eps;
        reset();
    }

    DBSCAN::~DBSCAN(){}

    void DBSCAN::init(float eps, size_t min_elems)
    {
        // given eps, convert to eps_sqr for convenience
        m_eps_sqr = eps * eps;
        m_min_elems = min_elems;
    }

    void DBSCAN::reset(){
        m_labels.clear();
    }

    const Labels & DBSCAN::get_labels() const{
        return m_labels;
    }

    void DBSCAN::output_result(const std::string filename) const {
        uint32_t size_data = cl_d.size1();
        uint32_t size_feature = cl_d.size2();
        uint32_t size_ans = m_labels.size();
        assert( size_data == size_ans);

        // map the cluster result into number from 0 to max
        std::ofstream fout(filename.data());
        for(uint32_t i=0; i < size_data; i++){
            for(uint32_t j=0; j<size_feature; j++)
                fout<<cl_d(i, j)<<" ";
            fout<<m_labels[i]<<endl;
        }
        fout.close();
    }

    void DBSCAN::reshape_labels(){
        // the answer of the previews version of reshape_labels may be influenced by the sequence of the answer
        
        unsigned int size_data = m_labels.size();
        int index = 0;
        std::map<int, int> mapping;
        for(uint32_t i=0; i<size_data; i++){
            if(m_labels[i] == -1)
                continue;
            if(mapping.find(m_labels[i]) == mapping.end()){
                mapping.insert(std::make_pair(m_labels[i], index));
                index++;
            }
            m_labels[i] = mapping.find(m_labels[i])->second;
        }
        cout<<"get "<<mapping.size()<<" clusters"<<endl;
    }

    void DBSCAN::prepare_labels( size_t s ){
        m_labels.resize(s);
		// icpc 12.1.4 does not support auto
        //for( auto & l : m_labels)
        //    l = -1;
		for(unsigned int i=0; i<m_labels.size(); i++)
			m_labels[i] = -1;
    }

}
