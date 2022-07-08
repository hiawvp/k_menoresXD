#include <iostream>
#include <ctime>       /* clock_t, clock, CLOCKS_PER_SEC */
#include "include/BasicCDS.h"
#include <vector>
#include <stack>
#include <algorithm>
#include <cstring>
#include <stdlib.h>

#define MAX_VAL 4891234
#define PRINT_LIMIT 220
#define RANDOM_SEED 6969

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


vector<long> generate_array(size_t n, int seed);
template<typename T>
void print_vector(vector<T> &v);

template<typename T>
void print_vector_segment(vector<T> &v, size_t i, size_t j);

template<typename T>
void print_vector_block(vector<T> &v, size_t b);


template<typename T>
void run_count_encode(vector<T> &v, size_t i, size_t j);
template<typename T>
void run_count_decode(vector<T> &v, size_t i, size_t j);


template<typename T>
vector<vector<T>> get_all_m_minors(vector<T> &v, size_t b, size_t m);

template<typename T>
vector<T> find_m_minors(vector<T> &block, size_t m);

vector<long> get_k_minors(vector<long> input, size_t i, size_t j, size_t k);
template<typename T>
vector<T> get_k_minors(vector<vector<T>> all_ms, size_t i, size_t j, size_t k, size_t b, vector<T> &v, size_t m);

//count_last
template<typename T>
struct bolsa{
    size_t idx = 0;
    vector<T> mins;
    size_t block_start_idx = 0;
    size_t block_len = 0;
};

template<typename T>
vector<T> k_mins_from_blocks(vector<bolsa<T>> &sakura, size_t k);

int main( int argc, char **argv ) { 
    if (argc != 6){
        fprintf(stderr, "error, ejecutar como: ./prog N b i j k\n");
        exit(EXIT_FAILURE);
        }
    size_t n = atoi(argv[1]);
    size_t b = atoi(argv[2]);	
    size_t i = atoi(argv[3]);
    size_t j = atoi(argv[4]);
    size_t k = atoi(argv[5]);

    size_t m = 2;
    // srand(RANDOM_SEED);
    size_t w = 4;
    // for(size_t w = 0; w < 10; w++){
        vector<long> rand_vec = generate_array(n, w);
        print_vector_block(rand_vec, b);
        vector<vector<long>> all_m_minors = get_all_m_minors(rand_vec, b, m);
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



vector<long> generate_array(size_t n, int seed){
		srand(seed);
    vector<long> output(n);
    for (size_t i = 0; i < n; i++){
        output[i] = rand() % PRINT_LIMIT; 
        }
    return output;
    }

template<typename T>
void print_vector(vector<T> &v){
    if (v.size() > PRINT_LIMIT) return;
    for (auto val : v){
        cout << val << " ";
        }    
    cout << endl;
    }

template<typename T>
void print_vector_segment(vector<T> &v, size_t i, size_t j){
    if (v.size() > PRINT_LIMIT) return;

    for (size_t l = 0; l < v.size(); l++){
        if(l == i){
            cout << "| ";
            }
        cout << v[l] << " ";
        if(l == j){
            cout << "| ";
            }
        }    
    cout << endl;
}

template<typename T>
void print_vector_block(vector<T> &v, size_t b){
    if (v.size() > PRINT_LIMIT) return;

    for (size_t l = 0; l < v.size(); l++){
        if(l % b == 0){
            cout << " | ";
        }
        cout << v[l] << " ";
    }    
    cout << endl;
}

vector<long> get_k_minors(vector<long> input, size_t i, size_t j, size_t k){
    size_t segment_size = j - i;
    if (segment_size < k) exit(0);
    vector<long> vec_copy(segment_size);
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
    sort_heap(m_minors_heap.rbegin(), m_minors_heap.rend());
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

    vector<T> aaa = find_m_minors(first, min(m, first.size()));
    sakura.push_back(bolsa<T>{0, aaa, i, prefix_len});	

    for(size_t l = first_block_idx; l <= last_block_idx; l++){
        sakura.push_back(bolsa<T>{0, all_ms[l], b*l, b});
        // print_vector(all_ms[l]); 
    }
	
    vector<T> bbb = find_m_minors(last, min(m, last.size()));
    sakura.push_back(bolsa<T>{0, bbb, j - suffix_len, suffix_len});	

    vector<T> kmins = k_mins_from_blocks(sakura, k, v);

    // print_vector(kmins);
    return kmins;
}


template<typename T>
vector<T> k_mins_from_blocks(vector<bolsa<T>> &sakura, size_t k, vector<T> &v){
	vector<T> kmins(k);
    for (size_t ii = 0; ii < k; ii++){
        size_t current_min_idx = -1;
        kmins[ii] = MAX_VAL;
        for (size_t l = 0; l < sakura.size(); l++){
            auto temp = sakura[l];
            // ya consumimos los mins del bloque
            // puede que hayan otros kmins
            // contar cuantas veces se encuentra el ultimo
            // encontrar el m+q menor
            T current_val; 
            if (temp.idx >= temp.mins.size() && temp.idx < temp.block_len){
                vector<T> temp_block(temp.block_len);
                for(size_t w = 0; w < temp.block_len; w++){
                    temp_block[w] = v[temp.block_start_idx + w];
                }
                auto m = temp_block.begin() + temp.idx;
                nth_element(temp_block.begin(), m, temp_block.end()); 
                current_val = temp_block[temp.idx];
            } 
            else{
                //convertir
                current_val = temp.mins[temp.idx];
            }
            if(current_val <= kmins[ii]){
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