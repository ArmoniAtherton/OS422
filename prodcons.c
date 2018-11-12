/*
 *  prodcons module
 *  Producer Consumer module
 * 
 *  Implements routines for the producer consumer module based on 
 *  chapter 30, section 2 of Operating Systems: Three Easy Pieces
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// Include only libraries for this module
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"

//Test Will This be thread safe???? //THINK IT WILL
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
int produced = 0;
int consumed = 0;

// Define Locks and Condition variables here 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty, fill = PTHREAD_COND_INITIALIZER;

// Producer consumer data structures
Matrix * bigmatrix[MAX];


// Bounded buffer put() get()
int put(Matrix * value)
{
  *(bigmatrix + fill_ptr) = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
  printf("PUT, COUNT: %d\n", count);
  produced++;
  return NULL;
}

Matrix * get() 
{
  Matrix * tmp = *( bigmatrix + use_ptr);
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  printf("GET, COUNT: %d\n", count);
  consumed++;
  return tmp;
}

void *prod_worker(void *arg)
{
  int i = 0;
  for (i = 0; i < LOOPS; i++) {
    pthread_mutex_lock(&lock);
    while (count == MAX) {
        pthread_cond_wait(&empty, &lock); 
    }
    put(GenMatrixRandom());
    pthread_cond_signal(&fill); 
    pthread_mutex_unlock(&lock);

    }
    return NULL;
}

// Matrix CONSUMER worker thread
void *cons_worker(void *arg)
{
  int i = 0;
  for (i = 0; i < LOOPS; i++) {
    pthread_mutex_lock(&lock);
    while (count == 0) {
        pthread_cond_wait(&fill, &lock); 
    }
    
    Matrix * tmp1 = get();
    // printf("THIS is Produced: %d, Consumed: %d, %d\n", produced, consumed, (produced - consumed)); 
    while (tmp1 == NULL) {
          FreeMatrix(tmp1);
          tmp1 = get();
    }
    pthread_cond_signal(&empty); 
    pthread_mutex_unlock(&lock); 

    //Print the matrix.
    DisplayMatrix(tmp1, stdout);
    FreeMatrix(tmp1);
    printf("\n");
  }
  return NULL; 
}


    // int flag = 1;
    // printf("THIS is Produced: %d, Consumed: %d, %d\n", produced, consumed, (produced - consumed)); 
    // while (tmp2 == NULL && flag) {
    //   if ((produced - consumed) < 1) {
    //     printf("FLAG SET:\n");
    //     flag = 0;
    //   } else {
    //     printf("IN HEREEEEE\n");
    //     FreeMatrix(tmp2);
    //     tmp2 = get();
    //   }
    // }
    // printf("OUT OF LOOP");
    // //This will check if we only have one matrix left in the bigmatrix.
    // if (flag) {
    //   printf("MULTIPLY: ");
    //   Matrix * tmp3 = MatrixMultiply(tmp1, tmp2);
    //   FreeMatrix(tmp1);
    //   FreeMatrix(tmp2);
    //   // DisplayMatrix(tmp3,stdout);
    //   FreeMatrix(tmp3);
    //   //Only on matrix left in bigmatrix
    // } else {
    //   printf("ONLY WHEN ONE LEFT: ");
    //   DisplayMatrix(tmp1, stdout);
    //   FreeMatrix(tmp1);
    // }
    // pthread_cond_signal(&empty); 
    // pthread_mutex_unlock(&lock); 

    // //Print the matrix.
    // // DisplayMatrix(tmp1, stdout);
    // // DisplayMatrix(tmp3, stdout);
    // printf("\n");

