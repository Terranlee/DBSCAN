#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>
#include <time.h>
#include <cassert>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <vector>

#include "dbscan.h"

namespace clustering
{
	DBSCAN::ClusterData DBSCAN::read_cluster_data(size_t features_num, size_t elements_num, std::string filename){
		DBSCAN::ClusterData cl_d( elements_num, features_num );
		std::ifstream fin(filename.data());
		for(size_t i=0; i<elements_num; i++)
			for(size_t j=0; j<features_num; j++)
				fin>>cl_d(i, j);
		fin.close();
		/*
		for(size_t i=0; i<10; i++){
			for(size_t j=0; j<features_num; j++)
				std::cout<<cl_d(i, j)<<" ";
			std::cout<<std::endl;
		}
		*/
		return cl_d;
	}

	DBSCAN::ClusterData DBSCAN::gen_cluster_data( size_t features_num, size_t elements_num ){
		DBSCAN::ClusterData cl_d( elements_num, features_num );
		for (size_t i = 0; i < elements_num; ++i){
			for (size_t j = 0; j < features_num; ++j)
				cl_d(i, j) = (-1.0 + rand() * (2.0) / RAND_MAX);
		}
		return cl_d;
	}

	void DBSCAN::cmp_result(const Labels& a, const Labels& b){
		int cnt_right = 0;
		assert(a.size() == b.size());

		for(int i=0; i<a.size(); i++)
			if(a[i] == b[i])
				cnt_right++;
		double rate = (double)cnt_right / (double)a.size();
		std::cout<<"similarity: "<<rate<<std::endl;
	}

	double DBSCAN::get_clock(){
		clock_t t = clock();
		return double(t) / double(CLOCK_PER_SEC);
	}

	std::ostream& operator<<(std::ostream& o, DBSCAN & d)
	{
		for(const auto & l : d.get_labels())
			o << l << " ";
		o << std::endl;
		return o;
	}

	DBSCAN::DBSCAN(){	}

	DBSCAN::DBSCAN(double eps, size_t min_elems)
	:m_min_elems( min_elems ){
		// given eps, convert to eps_sqr for convenience
		m_eps_sqr = eps * eps;
		reset();
	}

	DBSCAN::~DBSCAN(){
	}

	void DBSCAN::init(double eps, size_t min_elems)
	{
		// given eps, convert to eps_sqr for convenience
		m_eps_sqr = eps * eps;
		m_min_elems = min_elems;
	}

	void DBSCAN::reset(){
		m_labels.clear();
	}

	void DBSCAN::prepare_labels( size_t s ){
		m_labels.resize(s);
		for( auto & l : m_labels)
			l = -1;
	}

	const DBSCAN::DistanceMatrix DBSCAN::calc_dist_matrix( const DBSCAN::ClusterData & C){
		DBSCAN::ClusterData cl_d = C;
		// rows x rows
		DBSCAN::DistanceMatrix d_m( cl_d.size1(), cl_d.size1() );
		for (size_t i = 0; i < cl_d.size1(); ++i){
			for (size_t j = i; j < cl_d.size1(); ++j){
				d_m(i, j) = 0.0;
				if (i != j){
					ublas::matrix_row<DBSCAN::ClusterData> U (cl_d, i);
					ublas::matrix_row<DBSCAN::ClusterData> V (cl_d, j);
					for (const auto e : ( U-V ) )
						d_m(i, j) += e * e;
					d_m(j, i) = d_m(i, j);
				}
			}
		}
		return d_m;
	}

	DBSCAN::Neighbors DBSCAN::find_neighbors_distance_matrix(const DBSCAN::DistanceMatrix & D, uint32_t pid){
		Neighbors ne;
		for (uint32_t j = 0; j < D.size1(); ++j){
			if 	( D(pid, j) <= m_eps_sqr )
				ne.push_back(j);
		}
		return ne;
	}

	void DBSCAN::expand_cluster_distance_matrix(DBSCAN::Neighbors & ne, const DBSCAN::DistanceMatrix & dm, const int cluster_id, const int pid){
		m_labels[pid] = cluster_id;
		for (uint32_t i = 0; i < ne.size(); ++i){
			uint32_t nPid = ne[i];
			if ( !m_visited[nPid] ){
				m_visited[nPid] = 1;
				Neighbors ne1 = find_neighbors_distance_matrix(dm, nPid);
				if ( ne1.size() >= m_min_elems ){
					for (const auto & n1 : ne1)
						ne.push_back(n1);
				}
			}
			if ( m_labels[nPid] == -1 )
				m_labels[nPid] = cluster_id;
		}
	}

	void DBSCAN::dbscan_distance_matrix( const DBSCAN::DistanceMatrix & dm ){
		m_visited.resize(dm.size1());
		for(std::vector<uint8_t>::iterator iter = m_visited.begin(); iter != m_visited.end(); ++iter)
			*iter = 0;

		uint32_t cluster_id = 0;
		for (uint32_t pid = 0; pid < dm.size1(); ++pid){
			if ( !m_visited[pid] ){  
				m_visited[pid] = 1;
				Neighbors ne = find_neighbors_distance_matrix(dm, pid );
				if (ne.size() >= m_min_elems){
					expand_cluster_distance_matrix(ne, dm, cluster_id, pid);
					++cluster_id;
				}
			}
		}
	}

	const DBSCAN::Labels & DBSCAN::get_labels() const{
		return m_labels;
	}

	void DBSCAN::output_result(const DBSCAN::ClusterData& cl_d, const std::string filename) const {
		uint32_t size_data = cl_d.size1();
		uint32_t size_feature = cl_d.size2();
		uint32_t size_ans = m_labels.size();
		assert( size_data == size_ans);

		std::ofstream fout(filename.data());
		for(uint32_t i=0; i < size_data; i++){
			for(uint32_t j=0; j<size_feature; j++)
				fout<<cl_d(i, j)<<" ";
			fout<<m_labels[i]<<std::endl;
		}
		fout.close();
	}

	// the following are for grid base method
	void DBSCAN::grid_init(const int features_num){
		double sq = sqrt(double(features_num));
		double eps = sqrt(m_eps_sqr);
		m_cell_width = eps / sq;
	}

	void DBSCAN::getMinMax_grid(const DBSCAN::ClusterData& cl_d, double* min_x, double* min_y, double* max_x, double* max_y){
		// TODO: dimension related function
		double maxx = maxy = numeric_limits<double>::min();
		double minx = miny = numeric_limits<double>::max();
		for(size_t i=0; i<cl_d.size1(); i++){
				if(cl_d(i,0) > maxx)
					maxx = cl_d(i,0);
				if(cl_d(i,0) < minx)
					minx = cl_d(i,0);
				if(cl_d(i,1) > maxy)
					maxy = cl_d(i,1);
				if(cl_d(i,1) < miny)
					miny = cl_d(i,1);
		}
		*max_x = maxx;
		*max_y = maxy;
		*min_x = minx;
		*min_y = miny;
	}

	void DBSCAN::hash_construct_grid(const DBSCAN::ClusterData& cl_d){
		// TODO: dimension related function
		int features_num = cl_d.size2();
		if(features_num != 2)
			std::cout<<"only 2D data supported now!"<<std::endl;
		grid_init(features_num);

		double min_x, min_y, max_x, max_y;
		getMinMax(cl_d, &min_x, &min_y, &max_x, &max_y);
		int nRows = int((max_x - min_x) / m_cell_width) + 1;
		int nCols = int((max_y - min_y) / m_cell_width) + 1;

		for(size_t i=0; i<cl_d.size1(); i++){
			int dx = int((cl_d(i,0) - min_x) / m_cell_width) + 1;
			int dy = int((cl_d(i,1) - min_y) / m_cell_width) + 1;
			int key = dx * (nCols + 1) + dy;

			std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(key);
			if(got == m_hash_grid.end()){
				std::vector<int> intvec;
				intvec.push_back(i);
				m_hash_grid.insert(intvec);
			}
			else
				got->second.push_back(i);
		}
		m_n_rows = nRows;
		m_n_cols = nCols;
	}

	void DBSCAN::search_in_neighbour(const ClusterData& cl_d, int point_id, int center_id){
		// TODO: dimension related function
		static const int num_neighbour = 21;
		int cell_iter = center_id - 2 * m_n_rows - 1;
		int counter = 0;
		for(int i=0; i<num_neighbour; i++){
			std::unordered_map<int, std::vector<int> >::const_iterator got = m_hash_grid.find(cell_iter);
			if(got != m_hash_grid.end()){
				for(int j=0; j<got->second.size(); j++){
					int which = got->second.at(j);

				}
			}
			if(counter >= m_min_elems)
				return true;

			cell_iter = cell_iter + 1;
			if(i == 2)			cell_iter = center_id - m_n_rows - 2;
			else if(i == 7)		cell_iter = center_id - 2;
			else if(i == 12)	cell_iter = center_id + m_n_rows - 2;
			else if(i == 17)	cell_iter = center_id +m_n_rows * 2 - 1;
		}
		return false;
	}

	void DBSCAN::determine_core_point_grid(const ClusterData& cl_d){
		m_is_core.resize(cl_d.size1());
		for(std::unordered_map<int, std::vector<int> >::const_iterator iter = m_hash_grid.begin(); iter != m_hash_grid.end(); ++iter){
			// TODO: > or >=
			if(iter->second.size() > m_min_elems){
				for(int i=0; i<iter->second.size(); i++){
					int which = iter->second.at(i);
					m_is_core[which] = true;
				}
			}
			else{
				int cell_id = iter->first;
				for(int i=0; i<iter->second.size(); i++){
					int point_id = iter->second.at(i);
					bool result = search_in_neighbour(cl_d, point_id, cell_id);
					m_is_core[point_id] = result;
				}
			}
		}
	}

	// two public fit interface 
	void DBSCAN::fit_distance_matrix( const DBSCAN::ClusterData & C ) {
		prepare_labels( C.size1() );
		const DBSCAN::DistanceMatrix D = calc_dist_matrix(C);
		dbscan_distance_matrix( D );
	}

	void DBSCAN::fit_grid_based(const DBSCAN::ClusterData& C){
		hash_construct_grid(C);
		determine_core_point_grid(C);

	}

}
