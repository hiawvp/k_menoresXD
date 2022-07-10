
#include <iostream>
#include <ctime>       /* clock_t, clock, CLOCKS_PER_SEC */
#include "include/BasicCDS.h"
#include "include/utils.h"
#include <vector>
#include <stack>
#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <math.h>

#ifndef ulong
#define ulong unsigned long
#endif

using std::cout;
using std::endl;
using std::vector;
using std::memcpy;
using std::sort;
using std::make_heap;
using std::pop_heap;
using std::push_heap;
using std::sort_heap;
using std::stack;
using std::min;
using std::nth_element;
using std::max_element;

using namespace cds;
using namespace utils;




template<typename T>
vector<T> run_count_encode(vector<T> &v);
template<typename T>
vector<T> run_count_decode(vector<T> &v);


template<typename T>
vector<vector<T>> get_all_m_minors(vector<T> &v, size_t b, size_t m);

template<typename T>
vector<T> find_m_minors(vector<T> &block, size_t m);

template<typename T>
vector<T> get_k_minors(vector<T> input, size_t i, size_t j, size_t k);

template<typename T>
vector<T> get_k_minors(vector<vector<T>> all_ms, size_t i, size_t j, size_t k, size_t b, vector<T> &v, size_t m);

template<typename T>
vector<vector<T>> compare_solutions(vector<T> &sol_a, vector<T> &sol_b);

template<typename T>
vector<ulong> succinct_encode(vector<T> &v);

vector<ulong> succinct_decode(vector<ulong> &X, uint max_size, size_t m);
//count_last

template<typename T>
struct bolsa{
    size_t idx = 0;
    size_t n_mins; // [1 1] m elementos
    size_t block_start_idx = 0;
    size_t block_len = 0;
    vector<ulong> succinct;
    size_t n_bits;
};

template<typename T>
vector<T> k_mins_from_blocks(vector<bolsa<T>> &sakura, size_t k);

int main( int argc, char **argv ) { 
    if (argc != 6){
        fprintf(stderr, "Error, ejecutar como: ./prog N b i j k\n");
        exit(EXIT_FAILURE);
		}
    size_t n = atoi(argv[1]);
    size_t b = atoi(argv[2]);	
    size_t i = atoi(argv[3]);
    size_t j = atoi(argv[4]);
    size_t k = atoi(argv[5]);
	
    size_t m =  floor(log(n));
	
	cout << "Valor de log(n): " << m <<endl; 
    // srand(RANDOM_SEED);
    size_t w = 2;
    // for(size_t w = 0; w < 10; w++){
        vector<ulong> rand_vec = generate_array(n, w);
        print_vector_block(rand_vec, b);
        vector<vector<ulong>> all_m_minors = get_all_m_minors(rand_vec, b, m);
        clock_t t;
        t = clock();
        auto k_menores_pro = get_k_minors(all_m_minors, i, j, k, b, rand_vec, m);
        t = clock() - t;
        float total_time = (float)t/CLOCKS_PER_SEC;
        cout << "timeours: " << total_time << "secs" << endl;
        t = clock();
        auto k_menores_sort = get_k_minors(rand_vec, i, j,  k);
        t = clock() - t;
        total_time = (float)t/CLOCKS_PER_SEC;
        cout << "timexd: " << total_time << "secs" << endl;
        //cout << "timexd: " << total_time << "secs" << endl;
        print_vector(k_menores_pro);
        cout << "solucion real: " << endl;
        print_vector(k_menores_sort);
        cout << "--------------------------------------" << endl;
    // }
    return 0;
    }


template<typename T>
vector<T> get_k_minors(vector<T> input, size_t i, size_t j, size_t k){
    size_t segment_size = j - i;
    if (segment_size < k) exit(0);
    vector<T> vec_copy(segment_size);
    for(size_t l = 0; l < segment_size; l++){
        vec_copy[l] = input[i + l]; 
        }
    sort(vec_copy.begin(), vec_copy.end());
    vec_copy.resize(k); 
    return vec_copy;
}

template<typename T>
vector<T> find_m_minors(vector<T> &block, size_t m){
    // cout << "M : " << m << endl;
    vector<T> m_minors_heap(m);
		//No se cae xd
    for(size_t l = 0; l < m; l++){
        m_minors_heap[l] = block[l]; 
    }
    make_heap(m_minors_heap.begin(), m_minors_heap.end());
    // cout << "heap size: " << m_minors_heap.size() << endl;
    for(size_t l = m; l < block.size(); l++){
        if (m_minors_heap.front() > block[l]){
            pop_heap(m_minors_heap.begin(), m_minors_heap.end());
            m_minors_heap.pop_back();
            m_minors_heap.push_back(block[l]);
            push_heap(m_minors_heap.begin(),m_minors_heap.end());
        }
        // cout << "heap size: " << m_minors_heap.size() << endl;
    }
    sort_heap(m_minors_heap.begin(), m_minors_heap.end());
    return m_minors_heap;
}


template<typename T>
vector<vector<T>> get_all_m_minors(vector<T> &v, size_t b, size_t m){
    vector<vector<T>> all_ms;
    vector<T> block(b);
    for(size_t i = 0; i < v.size(); i+= b){
        for(size_t j = 0; j < b; j++){
            block[j] = v[i + j]; 
        }
        auto block_minors = find_m_minors(block, m);
				
        all_ms.push_back(block_minors);
        block_minors.clear();
    }
    return all_ms;
}
template<typename T>
bolsa<T> get_cool_bag(vector<T> v, size_t m, size_t i, size_t block_len){
    vector<T> m_mins = find_m_minors(v, min(m, v.size()));
    // print_vector(m_mins);
    auto gap_coded = run_count_encode(m_mins);
    auto succed = succinct_encode(gap_coded);
    auto _max = max_element(gap_coded.begin(), gap_coded.end());
    int max_elem = *_max;
    ulong max_size = 1 + log2(max_elem);
    bolsa<T> bag = bolsa<T>{0, m_mins.size(), i, block_len, succed, max_size};
    return bag;
}

template<typename T>
vector<T> get_k_minors(vector<vector<T>> all_ms, size_t i, size_t j, size_t k, size_t b, vector<T> &v, size_t m){

    size_t first_block_idx = (i / b) + (int)(i % b != 0);
    size_t prefix_len = 0;
    size_t suffix_len = 0;
    if (first_block_idx != 0){
        prefix_len = (first_block_idx * b) - i;
		}
    size_t last_block_idx = (j / b) - (int)(j % b != b-1);
    if (last_block_idx != all_ms.size() - 1 || j % b == b-1){
        suffix_len = j - (((last_block_idx + 1) * b) - 1);
		}
    // cout << "first_idx:  " << first_block_idx  << endl;
    // cout << "last_idx:  " << last_block_idx  << endl;
    // cout << "prefix_len:  " << prefix_len << endl;
    // cout << "suffix_len:  " << suffix_len << endl;
    //block_m_mins and indice del menor elemento disponible
	
    vector<bolsa<T>> sakura;
    vector<T> first, last;
    for (size_t w = i; w < (i + prefix_len); w++ ){
        first.push_back(v[w]);
        }
	
    for (size_t w = j; w > ( j - suffix_len); w-- ){
        last.push_back(v[w]);
        }
    // cout << "encoded gaps: " << endl;
    auto cool_bag = get_cool_bag(first, m, i, prefix_len);
    sakura.push_back(cool_bag);	

    for(size_t l = first_block_idx; l <= last_block_idx; l++){
        auto cool_bag = get_cool_bag(all_ms[l], m, b*l, b);
        sakura.push_back(cool_bag);	
    }
    cool_bag = get_cool_bag(last, m, j - suffix_len, suffix_len);
    sakura.push_back(cool_bag);	
    vector<T> kmins = k_mins_from_blocks(sakura, k, v);
    return kmins;
}


template<typename T>
vector<T> k_mins_from_blocks(vector<bolsa<T>> &sakura, size_t k, vector<T> &v){
	vector<T> kmins(k);
	// cout << "decode gapsXD:" << endl;
 //    for (size_t l = 0; l < sakura.size(); l++){
 //        auto temp = sakura[l];
 //        vector<ulong> gap_decoded = succinct_decode(temp.succinct,
 //        temp.n_bits,
 //        temp.n_mins);
 //        // print_vector(gap_decoded);
 //        auto cxvasd = run_count_decode(gap_decoded);
 //        // print_vector(cxvasd);
 //    }
	// 	cout << "run_count_decode" << endl;
	// 	// print_vector(v);
	// 	// exit(0);
    for (size_t ii = 0; ii < k; ii++){
        size_t current_min_idx = -1;
        kmins[ii] = MAX_VAL;
        T current_val = MAX_VAL; 
        for (size_t l = 0; l < sakura.size(); l++){
            auto temp = sakura[l];
            if (temp.idx < temp.n_mins){
                // current_val = temp.mins[temp.idx];
                 vector<ulong> gap_decoded = succinct_decode(temp.succinct,
                                                     temp.n_bits,
                                                     temp.n_mins);

                vector<T> coochie = run_count_decode(gap_decoded);
                current_val = coochie[temp.idx];
            }
            else if (temp.idx < temp.block_len){
                vector<T> temp_block(temp.block_len);
                for(size_t w = 0; w < temp.block_len; w++){
                    temp_block[w] = v[temp.block_start_idx + w];
                }
                auto m = temp_block.begin() + temp.idx;
                nth_element(temp_block.begin(), m, temp_block.end()); 
                current_val = temp_block[temp.idx];
            } 
            if(current_val < kmins[ii]){
                // cout << kmins[ii] << " XD, ii: " << ii <<  endl;
                kmins[ii] = current_val;
                current_min_idx = l;
                // cout << kmins[ii] << " XD, ii: " << ii <<  endl;
            }
        }
        sakura[current_min_idx].idx++;
        }

    return kmins;
}

template<typename T>
void compare_solutions(vector<T> &sol_a, vector<T> &sol_b, size_t k){
   if(sol_a.size() != k){
       cout << "sol_a no tiene k elementos" << endl;
       return;
   } 
   if(sol_b.size() != k){
       cout << "sol_a no tiene k elementos" << endl;
       return;
   } 
}


template<typename T>
vector<T> run_count_encode(vector<T> &v){
    vector<T> gapscoding;
    auto sum = v[0];
    gapscoding.push_back(sum);
    for (size_t i = 1; i < v.size(); i++){
        auto temp = v[i] - sum;
        gapscoding.push_back(temp);
        sum += temp;
    }
    return gapscoding;
}


template<typename T>
vector<T> run_count_decode(vector<T> &v){
		// print_vector(v);
    vector<T> decoded;
    auto sum = v[0];
    decoded.push_back(sum);
    for (size_t i = 1; i < v.size(); i++){
        // auto temp = sum + v[i];
        sum += v[i];
        decoded.push_back(sum);
        // decoded.push_back(temp);
        // sum += temp;
    }
    return decoded;
}

    
////aca 
template<typename T>
vector<ulong> succinct_encode(vector<T> &v){
	ulong i, j;
	auto max = max_element(v.begin(), v.end());
	ulong max_elem = *max;
	uint max_size = 1 + log2(max_elem);
	// uint max_size = ceil(log2(max_elem));
    // cout << "max_size: " << max_size << " max_element " << max_elem << endl; 
	size_t nWX = (v.size()*max_size)/(sizeof(ulong)*8);
    if ((v.size()*max_size)%(sizeof(ulong)*8)>0) nWX++;
	vector<ulong> X(nWX);
	for (i=j=0; i<v.size(); i++, j+=max_size){
		setNumV64(X, j, max_size, (ulong)v[i]);
	}
	return X;
} 

////aca 
vector<ulong> succinct_decode(vector<ulong> &X, uint max_size, size_t m){
	ulong i, j;
	vector<ulong> succ_decoded;
    for (i=j=0; i<m; i++, j+=max_size){
        auto num = getNumV64(X, j, max_size);
        succ_decoded.push_back(num);
		}
    return succ_decoded;
} 


