#include <iostream>
#include <ctime>       /* clock_t, clock, CLOCKS_PER_SEC */
#include "include/BasicCDS.h"
#include <vector>
#include <algorithm>
#include <cstring>

#define MAX_VAL 69
#define PRINT_LIMIT 1000

using std::cout;
using std::endl;
using std::vector;
using std::memcpy;


vector<long> generate_array(size_t n, int seed);
template<typename T>
void print_vector(vector<T> &v);
vector<long> get_k_minors(vector<long> input, size_t i, size_t j, size_t k);


int main( int argc, char **argv ) { 
    if (argc != 2){
        fprintf(stderr, "error, ejecutar como: ./prog N \n");
        exit(EXIT_FAILURE);
        }
    size_t n = atoi(argv[1]);
    
  auto rand_vec = generate_array(n, 0);
    print_vector(rand_vec);
    clock_t t;
    t = clock();

    t = clock() - t;
    float total_time = (float)t/CLOCKS_PER_SEC;
    return 0;
    }


vector<long> generate_array(size_t n, int seed){
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

vector<long> get_k_minors(vector<long> input, size_t i, size_t j, size_t k){
    size_t segment_size = j - i;
    if (segment_size < k) exit(0);
    //memcpy();   
    vector<long> vec_copy(j - i);    
		return vec_copy;
}

