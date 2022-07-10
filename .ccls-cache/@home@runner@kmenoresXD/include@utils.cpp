#include "utils.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

namespace utils {
    vector<ulong> generate_array(size_t n, int seed){
        srand(seed);
        vector<ulong> output(n);
        for (size_t i = 0; i < n; i++){
            output[i] = rand() % PRINT_LIMIT; 
            }
        return output;
    }
    
    void print_vector(vector<ulong> &v){
        if (v.size() > PRINT_LIMIT) return;
        for (auto val : v){
            cout << val << " ";
            }    
        cout << endl;
    }

    void print_vector(vector<int> &v){
        if (v.size() > PRINT_LIMIT) return;
        for (auto val : v){
            cout << val << " ";
            }    
        cout << endl;
    }
    void print_vector_segment(vector<ulong> &v, size_t i, size_t j){
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
    void print_vector_segment(vector<int> &v, size_t i, size_t j){
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
    void print_vector_block(vector<ulong> &v, size_t b){
        if (v.size() > PRINT_LIMIT) return;
    
        for (size_t l = 0; l < v.size(); l++){
            if(l % b == 0){
                cout << " | ";
            }
            cout << v[l] << " ";
        }    
        cout << endl;
    }
    void print_vector_block(vector<int> &v, size_t b){
        if (v.size() > PRINT_LIMIT) return;
    
        for (size_t l = 0; l < v.size(); l++){
            if(l % b == 0){
                cout << " | ";
            }
            cout << v[l] << " ";
        }    
        cout << endl;
    }
} /* namespace cds */

