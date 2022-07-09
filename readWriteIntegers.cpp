#include <iostream>
#include <random>
#include <ctime>       /* clock_t, clock, CLOCKS_PER_SEC */
#include "include/BasicCDS.h"

using namespace std;
using namespace cds;

#define PRINT 0
#define TEST 1

uint bM; 		// bits for MAX

// Structure with all globals parameters program
typedef struct {
	ulong *A;
	ulong *X;
	ulong n;
	ulong MAX;

	ulong sizeA, sizeX;
	ulong nWX;		// number of Words for X[]
} ParProg;

void genArrays(ParProg *par);

/*
Aveces el entero se lee una sola vez y luego se trabaja con una variavble int o long int, por tanto el tiempo que se pierde es despreciable respecto a lo que se gana en espacio cuando los enteros son relativamente pequeños.
- probar utilizar 5 bytes para numeros grandes
*/

int main(int argc, char** argv){
	ParProg *par = new ParProg();

	if(argc != 3){
		cout << "Execution Error! call: ./readWriteInt <n> <MAX>" << endl;
		exit(EXIT_FAILURE);
	}
	par->n = atoi(argv[1]);
	par->MAX = atoi(argv[2]);

	cout << "Parameters..." << endl;
	cout << "n = " << par->n << endl;
	cout << "MAX = " << par->MAX << endl;

	genArrays(par);

	cout << "################## " << endl;
	return 0;  
}


void genArrays(ParProg *par){
	ulong i, j, k;

	par->A = new ulong[par->n];
	par->sizeA = par->n*sizeof(ulong);
	for (i=0; i<par->n; i++)
		par->A[i] = rand()%par->MAX;

	// **************************************************************************
	// create X[] array...
	bM = 1+log2(par->MAX);
	par->nWX = (par->n*bM)/(sizeof(ulong)*8); // number of words for X
	if ((par->n*bM)%(sizeof(ulong)*8)>0)
		par->nWX++;

	par->X = new ulong[par->nWX];
	par->sizeX = par->nWX*sizeof(ulong);

	cout << "bM = " << bM << endl;
	cout << "nWX = " << par->nWX << endl;
	cout << " size for A[] = " << par->sizeA/(1024.0*1024.0) << " MiB (using ulong per cell)" << endl;
	cout << " size for X[] = " << par->sizeX/(1024.0*1024.0) << " MiB" << endl;

	// store values from A[] into X[] (calling the method setNum64())...
	for (i=j=0; i<par->n; i++, j+=bM)
		setNum64(par->X, j, bM, par->A[i]);

	if (PRINT){
		cout << "A[] = ";
		for (i=0; i<par->n; i++)
			cout << par->A[i] << " ";
		cout << endl;

		cout << "X[] = ";
		//read values from X[]...
		for (i=j=0; i<par->n; i++, j+=bM){
			cout << getNum64(par->X, j, bM) << " ";
		}
		cout << endl;

		// print bits using printBitsUlong()...
		for (i=0; i<par->nWX; i++){
			printBitsUlong(par->X[i]);
			cout << " - ";
		}
		cout << endl;
	}

	if(TEST){
		// check all the position A[i] == X[i]...
		for (i=j=0; i<par->n; i++, j+=bM){
			k = getNum64(par->X, j, bM);
			if (k!=par->A[i]){
				cout << "ERROR. A[" <<i<< "] = " << par->A[i] << " != X[i] = " << k << endl;
				exit(-1);
			}
		}

		cout << "Test OK !!" << endl;
	}
}
