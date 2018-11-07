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

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// int ready = 0;

int main (int argc, char * argv[])
{
  time_t t;
  int numw = NUMWORK;

  // Seed the random number generator with the system time
  srand((unsigned) time(&t));

  //
  // Demonstration code to show the use of matrix routines
  //
  // DELETE THIS CODE ON ASSIGNMENT 2 SUBMISSION
  // ----------------------------------------------------------
  // printf("MATRIX MULTIPLICATION DEMO:\n\n");
  // Matrix *m1, *m2, *m3;
  // for (int i=0;i<12;i++)
  // {
  //   m1 = GenMatrixRandom();
  //   m2 = GenMatrixRandom();
  //   m3 = MatrixMultiply(m1, m2);
  //   if (m3 != NULL)
  //   {
  //     DisplayMatrix(m1,stdout);
  //     printf("    X\n");
  //     DisplayMatrix(m2,stdout);
  //     printf("    =\n");
  //     DisplayMatrix(m3,stdout);
  //     printf("\n");
  //     FreeMatrix(m3);
  //     FreeMatrix(m2);
  //     FreeMatrix(m1);
  //     m1=NULL;
  //     m2=NULL;
  //     m3=NULL;
  //   }
  // }
  // return 0;
  // ----------------------------------------------------------

  printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
  printf("Using a shared buffer of size=%d\n", MAX);
  printf("With %d producer and consumer thread(s).\n",numw);
  printf("\n");

  pthread_t pr;
  pthread_t co;

  int prs = 0;
  int cos = 0;
  int prodtot = 0;
  int constot = 0;
  int consmul = 0;

  // pthread_create(&pr, NULL, prod_worker, NULL);
  // pthread_create(&co, NULL, cons_worker, NULL);

  // cons_worker(NULL);

  // pthread_join(pr, NULL);
  // pthread_join(co, NULL);

  /** THIS WILL GEN MATRIX AND MULTIPLY **/
  // Matrix * bigmatrix[MAX];
  // *bigmatrix = AllocMatrix(ROW, COL);
  // *(bigmatrix + 0) = GenMatrixRandom();
  // *(bigmatrix + 1) = GenMatrixRandom();
  // // MatrixMultiply(*(bigmatrix + 0), *(bigmatrix + 1));
  // Matrix * n = MatrixMultiply(*(bigmatrix + 0), *(bigmatrix + 1));
  // while (n == NULL) {
  //     *(bigmatrix + 1) = GenMatrixRandom();
  //     n = MatrixMultiply(*(bigmatrix + 0), *(bigmatrix + 1));
  // }
  // DisplayMatrix(*(bigmatrix + 0), stdout);
  // DisplayMatrix(*(bigmatrix + 1), stdout);
  // DisplayMatrix(n, stdout);



  // consume ProdConsStats from producer and consumer threads
  // add up total matrix stats in prs, cos, prodtot, constot, consmul 

  printf("Sum of Matrix elements --> Produced=%d = Consumed=%d\n",prs,cos);
  printf("Matrices produced=%d consumed=%d multiplied=%d\n",prodtot,constot,consmul);
  return 0;
}
