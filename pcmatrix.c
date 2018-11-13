/*
 *  pcmatrix module
 *  Primary module providing control flow for the pcMatrix program
 * 
 *  Producer consumer bounded buffer program to produce random matrices in parallel
 *  and consume them while searching for valid pairs for matrix multiplication.
 *  Matrix multiplication requires the first matrix column count equal the 
 *  second matrix row count.  
 *
 *  A matrix is consumed from the bounded buffer.  Then matrices are consumed
 *  from the bounded buffer, one at a time, until an eligible matrix for multiplication
 *  is found.
 *
 *  Totals are tracked using the ProdConsStats Struct for:
 *  - the total number of matrices multiplied (multtotal from consumer threads)
 *  - the total number of matrices produced (matrixtotal from producer threads)
 *  - the total number of matrices consumed (matrixtotal from consumer threads)
 *  - the sum of all elements of all matrices produced and consumed (sumtotal from producer and consumer threads)
 *
 *  Correct programs will produce and consume the same number of matrices, and
 *  report the same sum for all matrix elements produced and consumed. 
 *
 *  For matrix multiplication only ~25% may be e
 *  and consume matrices.  Each thread produces a total sum of the value of
 *  randomly generated elements.  Producer sum and consumer sum must match.
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"
#include "counter.h"
#include "prodcons.h"
#include "pcmatrix.h"

int main (int argc, char * argv[]) {
  time_t t;
  int numw = NUMWORK;
  // Seed the random number generator with the system time
  srand((unsigned) time(&t));

// Data structure to track matrix production / consumption stats
// sumtotal - total of all elements produced or consumed
// multtotal - total number of matrices multipled 
// matrixtotal - total number of matrces produced or consumed
ProdConsStats * stats_prod = malloc(sizeof(ProdConsStats));
stats_prod->sumtotal = 0; stats_prod->multtotal = 0; stats_prod->matrixtotal = 0;

ProdConsStats * stats_con = malloc(sizeof(ProdConsStats));
stats_con->sumtotal = 0; stats_con->multtotal = 0; stats_con->matrixtotal = 0;
/* ***************** 
only need one struct for both I think. But would have to modify given struct
***************** */ 

  pthread_t pr[NUMWORK];
  pthread_t co[NUMWORK];

  for(int i = 0; i < NUMWORK; i++) {
    int p_worker = pthread_create(&pr[i], NULL, prod_worker, stats_prod);
    int c_worker = pthread_create(&co[i], NULL, cons_worker, stats_con);
    assert(p_worker == 0);
    assert(c_worker == 0);
  }  

   for(int i = 0; i < NUMWORK; i++) {
    int p_worker = pthread_join(pr[i], (void *) &stats_prod);
    int c_worker = pthread_join(co[i], (void *) &stats_con);
    assert(p_worker == 0);
    assert(c_worker == 0);
  } 

  int prs = stats_prod->sumtotal;
  int cos = stats_con->sumtotal;
  int prodtot = stats_prod->matrixtotal;
  int constot = stats_con->matrixtotal;
  int consmul = stats_con->multtotal;

  // printf("multiplied %d\n",stats_con->multtotal);

  // consume ProdConsStats from producer and consumer threads
  // add up total matrix stats in prs, cos, prodtot, constot, consmul 
  printf("Sum of Matrix elements --> Produced=%d = Consumed=%d\n",prs,cos);
  printf("Matrices produced=%d consumed=%d multiplied=%d \n",
      prodtot,constot,consmul);

  // free(&stats_prod);
  // free(&stats_con);
  return 0;
}
