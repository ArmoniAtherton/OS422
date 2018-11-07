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
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty, fill = PTHREAD_COND_INITIALIZER;
int ready = 0;

// Producer consumer data structures
Matrix * bigmatrix[MAX];

// Bounded buffer put() get()
int put(Matrix * value)
{
  return NULL;
}

Matrix * get() 
{
  return NULL;
}

// Matrix PRODUCER worker thread
void *prod_worker(void *arg)
{
  int i = 0;
  for (i = 0; i < LOOPS; i++) {

    // Pthread_mutex_lock(&lock);
   
   
    // // signal parent
    // // ready=1;
    // // pthread_cond_signal(&cond);
    // while (ready == 1) {//SHOULD BE READY == MAX(PRODUCERS)
    //     Pthread_cond_wait(&empty, &lock); 
    // }
    // //NEED TO GET MATRIX AND PRINT IT OUT.
    // ready = 0;
    // Pthread_cond_signal(&fill); 
    // Pthread_mutex_unlock(&lock); 
    //NEED TO FREE MATRIX
    
    }
    return NULL;
}
// void *producer(void *arg) {
//   5 int i;
//   6 for (i = 0; i < loops; i++) {
//   7 Pthread_mutex_lock(&mutex); // p1
//   8 while (count == MAX) // p2
//   9 Pthread_cond_wait(&empty, &mutex); // p3
//   10 put(i); // p4
//   11 Pthread_cond_signal(&fill); // p5
//   12 Pthread_mutex_unlock(&mutex); // p6
//   13 }
// 14 }

// Matrix CONSUMER worker thread
void *cons_worker(void *arg)
{
  // int i = 0;
  // for (i = 0; i < LOOPS; i++) {
  //   Pthread_mutex_lock(&lock);
  //   while (ready == 0) {
  //       Pthread_cond_wait(&fill, &lock); 
  //   }
  //   // int tmp = get(); 
  //   Pthread_cond_signal(&empty); 
  //   Pthread_mutex_unlock(&lock); 
  return NULL;
  // }
}


// void *consumer(void *arg) {
// 17 int i;
// 18 for (i = 0; i < loops; i++) {
// 19 Pthread_mutex_lock(&mutex); // c1
// 20 while (count == 0) // c2
// 21 Pthread_cond_wait(&fill, &mutex); // c3
// 22 int tmp = get(); // c4
// 23 Pthread_cond_signal(&empty); // c5
// 24 Pthread_mutex_unlock(&mutex); // c6
// 25 printf("%d\n", tmp);
// 26 }
