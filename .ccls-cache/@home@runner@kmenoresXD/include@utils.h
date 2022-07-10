
#ifndef UTILS_XD
#define UTILS_XD

#include <stdio.h>
#include <vector>
#include <ios>

using std::vector;

namespace utils {
	#ifndef uchar // 1 byte
	#define uchar unsigned char
	#endif
	#ifndef uint
	#define uint unsigned int
	#endif
	#ifndef ulong
	#define ulong unsigned long
	#endif

const ulong MAX_VAL = 4891234;
const ulong PRINT_LIMIT = 220;
const ulong RANDOM_SEED = 6969;

vector<ulong> generate_array(size_t n, int seed);

void print_vector(vector<ulong> &v);

void print_vector_segment(vector<ulong> &v, size_t i, size_t j);

void print_vector_block(vector<ulong> &v, size_t b);


void print_vector(vector<int> &v);

void print_vector_segment(vector<int> &v, size_t i, size_t j);

void print_vector_block(vector<int> &v, size_t b);
} /* namespace cds */

#endif /* BASIC_DRF_H_ */
