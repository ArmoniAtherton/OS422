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

// Define Locks and Condition variables here
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, 
               fill = PTHREAD_COND_INITIALIZER; 
// need a lock for producer put and one for consumer get ? ????????
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;
volatile int LOOPS_COUNTER_CON = LOOPS;
volatile int LOOPS_COUNTER_PROD = LOOPS;


//indexers for buffer
int fill_ptr = 0, use_ptr = 0;
int buffer_cnt = 0;


// Producer consumer data structures
Matrix * bigmatrix[MAX];

// Bounded buffer put() get()
int put(Matrix * value) {
  bigmatrix[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  buffer_cnt++;
  return 0;                     // what to return here??
}

Matrix * get()  {
  Matrix * temp = bigmatrix[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;  // should be = ?????
  buffer_cnt--;
  return temp;
}

// Matrix PRODUCER worker thread
void *prod_worker(void *arg) { 
  ProdConsStats *stats = (ProdConsStats *) arg;
  Matrix *temp;
  // printf("--  IN PRODUCER WORKER --\n"); 
  while (LOOPS_COUNTER_PROD > 0) {
    pthread_mutex_lock(&lock);

    // sleep if buffer is full
    while(buffer_cnt == MAX) {
      // printf("--  IN PRODUCER WHILE LOOP --\n"); 
      pthread_cond_wait(&empty, &lock); 
    }

    temp = GenMatrixRandom();
    put(temp);

    stats->sumtotal += SumMatrix(temp);

    // syncronize the threads to count to same loop iterations
    pthread_mutex_lock(&counter_lock);
    stats->matrixtotal += 1;
    LOOPS_COUNTER_PROD--;
    // printf("\n***********     %d ***************\n", stats->matrixtotal);
    pthread_mutex_unlock(&counter_lock);

    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&lock);
  }
  return stats;
} 

// Matrix CONSUMER worker thread
void *cons_worker(void *arg) {
  ProdConsStats *stats = (ProdConsStats *) arg;
  Matrix *m1 = 0, *m2 = 0, *m3 = 0;
  // printf("--  IN CONSUMER WORKER --\n");
  while (LOOPS_COUNTER_CON > 0) {
    pthread_mutex_lock(&lock);
    // printf("buff_cnt: %d\n", buffer_cnt);
    // printf("Start m1:%d  m2:%d m3:%d  i cnt:%d \n", 
      // (int) m1, (int) m2, (int) m3, i);

    // sleep if buffer is empty
    while(buffer_cnt == 0) {
      pthread_cond_wait(&fill, &lock); 
    }

    // Try to multiply while emptying the buffer
    if (m1 == 0 && m2 == 0) {
      m1 = get();
      stats->sumtotal += SumMatrix(m1);

      // syncronize the threads to count to same loop iterations
      pthread_mutex_lock(&counter_lock);
      stats->matrixtotal += 1;
      LOOPS_COUNTER_CON--;
      pthread_mutex_unlock(&counter_lock);

    } else if (m1 != 0 && m2 == 0) {
      m2 = get();
      stats->sumtotal += SumMatrix(m2);

      // syncronize the threads to count to same loop iterations
      pthread_mutex_lock(&counter_lock);
      stats->matrixtotal += 1;
      LOOPS_COUNTER_CON--;
      pthread_mutex_unlock(&counter_lock);

    } 
    if (m1 != 0 && m2 != 0) {
      m3 = MatrixMultiply(m1, m2);
      if (m3 != 0) { // could multiply matricies
        DisplayMatrix(m3, stdout);
        stats->multtotal += 1;
        FreeMatrix(m1); FreeMatrix(m3); 
        m1 = 0; m3 = 0;
      }
      FreeMatrix(m2); m2 = 0; 
    }

    //buffer only has one matrix left
    if (m1 != 0 && buffer_cnt == 0) {
      FreeMatrix(m1); m1 = 0;

      // syncronize the threads to count to same loop iterations
      pthread_mutex_lock(&counter_lock);
      stats->matrixtotal += 1;
      LOOPS_COUNTER_CON--;
      pthread_mutex_unlock(&counter_lock); 
    }

    // printf("End   m1:%d  m2:%d m3:%d  i cnt:%d \n\n", 
    //   (int) m1, (int) m2, (int) m3, i);
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&lock);
  }
  return stats; 
}


