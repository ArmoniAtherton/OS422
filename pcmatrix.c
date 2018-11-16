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


int main (int argc, char * argv[])
{
  time_t t;
  int numw = NUMWORK;

  // Seed the random number generator with the system time
  srand((unsigned) time(&t));

  printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
  printf("Using a shared buffer of size=%d\n", MAX);
  printf("With %d producer and consumer thread(s).\n",numw);
  printf("\n");

  //Intializes the struct to hold producer counts.
  ProdConsStats * prod_count = (ProdConsStats *) malloc(sizeof(ProdConsStats));
  prod_count->sumtotal = 0, prod_count->multtotal = 0, prod_count->matrixtotal = 0;

  //Intializes the struct to hold consumer counts.
  ProdConsStats * con_count = (ProdConsStats *) malloc(sizeof(ProdConsStats));
  con_count->sumtotal = 0, con_count->multtotal = 0, con_count->matrixtotal = 0;

  pthread_t pr[NUMWORK];
  pthread_t co[NUMWORK];

  //This will intialize a specified number of threads.
  for (int i = 0; i < NUMWORK; i++) {
    pthread_create(&pr[i], NULL, prod_worker, prod_count);
    pthread_create(&co[i], NULL, cons_worker, con_count);
  }

  //This will join a specified number of threads.
  for (int i = 0; i < NUMWORK; i++) {
    pthread_join(pr[i],  (void *) &prod_count);
    pthread_join(co[i], (void *) &con_count);  
  }

  int prs = prod_count->sumtotal;
  int cos = con_count->sumtotal;
  int prodtot = prod_count->matrixtotal;
  int constot = con_count->matrixtotal;
  int consmul = con_count->multtotal;

  printf("Sum of Matrix elements --> Produced=%d = Consumed=%d\n",prs,cos);
  printf("Matrices produced=%d consumed=%d multiplied=%d\n",prodtot,constot,consmul);

  free(prod_count);
  free(con_count);
  
  return 0; 
}












//  // initialize counters_t janky way
//   counters_t * c = (counters_t*) malloc(sizeof(counters_t));
//   counter_t *count = (counter_t *) malloc (sizeof(counter_t));
//   init_cnt(count);
//   c->prod = count;

//   // less janky way
//   counters_t * c2 = (counters_t*) malloc(sizeof(counters_t));
//   c2->cons = (counter_t *) malloc (sizeof(counter_t));
//   c2->prod = (counter_t *) malloc (sizeof(counter_t));
//   init_cnt(c2->cons);
//   init_cnt(c2->prod);

//   increment_cnt(c2->cons);

//   printf("***** %d %d *****\n", get_cnt(c2->cons), get_cnt(c2->prod));


  // counter_t * count = (counter_t *) malloc(sizeof(counter_t));
  // init_cnt(count);
  // increment_cnt(count);
  // printf("*********%d!!!!!!\n", get_cnt(count));


  /** THIS WILL GEN MATRIX AND MULTIPLY **/
  // Matrix * bigmatrix[MAX];
  // // *bigmatrix = AllocMatrix(ROW, COL);
  // *(bigmatrix + 0) = GenMatrixRandom();
  // *(bigmatrix + 1) = GenMatrixRandom();
  // // MatrixMultiply(*(bigmatrix + 0), *(bigmatrix + 1));
  // Matrix * n = MatrixMultiply(*(bigmatrix + 0), *(bigmatrix + 1));
  // while (n == NULL) {
  //     *(bigmatrix + 1) = GenMatrixRandom();
  //     n = MatrixMultiply(*(bigmatrix + 0), *(bigmatrix + 1));
  // }
  // DisplayMatrix(*(bigmatrix + 0), stdout);
  // printf("\n");
  // DisplayMatrix(*(bigmatrix + 1), stdout);
  // printf("\n");
  // DisplayMatrix(n, stdout);