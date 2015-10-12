#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SIZEOF 5		// side length of array
#define SPINUP 1
#define SPINDOWN -1

void init ( int **, int);
void getNeighbours (int **, int , int , int , int *);
int dE ( int **, int , int, int, int, int);
int flip(int, int, int, int *);
double samp ( int **, int, double *, double *);

int main ( int argc, char * argv[])
{
	int ** lattice;			// 2D array of spins
	double T = 1.0;			// Dimensionless temperature = (T*k)/J
	int J = 1;			// coupling constant
	int h = 0;			// magnetic field
	double k = 1.38;		// boltzman constant
	int *ptr;
	int deltaE; 			// Energy change due to flipping a spin
	int i, j;			// loop counters
	//int *ptr;			// pointer to go through elements
	int neighbours[4];		// 1D array to get nearest neighbours
	double P;			// Pflip
	double random;			// a random number

	srand(time(NULL));		// seed for random number generator

	lattice=(int **)malloc(SIZEOF*sizeof(int*));
	for (i=0; i<SIZEOF; i++)
		lattice[i]=(int *)malloc(SIZEOF*sizeof(int));

	init(lattice, SIZEOF);						//generate initial state
		
	for (i=0; i<SIZEOF; i++){					// print lattice to see if initilized correctly
		for (j=0; j<SIZEOF; j++){
			printf("%2d ", **lattice);
		}	printf("\n");
	}

	for (i=0; i<SIZEOF; i++){
		for (j=0; j<SIZEOF; j++){
			ptr=&(lattice[i][j]);				// sweeping through lattice to get pointers to element and a 
									// pointer to an array for the 4 nearest neighbours
			printf("The initial value of pointer is: %d \n", *ptr);
			getNeighbours(lattice, i, j, SIZEOF, neighbours);
			
			deltaE = dE(lattice, SIZEOF, i, j, J, h);
			printf("Delta E = %d \n", deltaE);

			P = exp(deltaE/(k*T));
			printf("P flip = %lf \n", P);
		
			random = (double) rand()/ (double) RAND_MAX;
			printf("Random number is: %lf \n", random);

			if(deltaE < 0){
				lattice[i][j] *= -1;
			}
			
			else if (deltaE > 0 && random < P){
				lattice[i][j] *= -1;
			}
		}
	}
		for(i=0; i<SIZEOF; i++){
			for(j=0; j<SIZEOF; j++){
				printf(" %2d", lattice[i][j]);
			}	printf("\n");
		} 
}





void init ( int ** L, int N)		// function to generate initial states
{
	int i, j;
	
	for (i=0; i<N; i++) {
		for (j=0; j<N; j++) {
			if(L[i][j] != SPINUP)
				L[i][j] = SPINUP;
		}
	}
}

void getNeighbours (int ** L, int i, int j, int N, int *nn)		// function to get nearest neighbour	
{									// Syntax: modulo operator ensures periodic boundary conditions
	nn[0] = L[i?(i-1):(N-1)][j];					// "i?(i-1):(N-1)" performs the following check, if i is non-zero
	nn[1] = L[(i+1)%N][j];						// return i-1, otherwise return N-1.
	nn[2] = L[i][j?(j-1):(N-1)];					// This too ensures periodic boundary conditions
	nn[3] = L[i][(j+1)%N];
}

int dE ( int ** L, int N, int i, int j, int J, int h ) {
 return -2*(L[i][j])*J*(L[i?(i-1):(N-1)][j]+L[(i+1)%N][j]+L[i][j?(j-1):(N-1)]+L[i][(j+1)%N] - h);
}