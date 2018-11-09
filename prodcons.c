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
  return NULL;
}
// 6 void put(int value) {
// 7   buffer[fill_ptr] = value;
// 8   fill_ptr = (fill_ptr + 1) % MAX;
// 9   count++;
// 10 }

Matrix * get() 
{
  Matrix * tmp = *( bigmatrix + use_ptr);
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

// 12 int get() {
// 13  int tmp = buffer[use_ptr];
// 14  use_ptr = (use_ptr + 1) % MAX;
// 15  count--;
// 16  return tmp;
// 17 }
// Matrix PRODUCER worker thread
void *prod_worker(void *arg)
{
  int i = 0;
  for (i = 0; i < LOOPS; i++) {
    pthread_mutex_lock(&lock);
    while (count == MAX) {//SHOULD BE READY == MAX(PRODUCERS)
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
    Matrix * tmp = get();
    pthread_cond_signal(&empty); 
    pthread_mutex_unlock(&lock); 

    //Print the matrix.
    DisplayMatrix(tmp, stdout);
    printf("\n");
  }
  return NULL;
}


