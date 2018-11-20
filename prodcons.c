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
#include <unistd.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"

//Will be the indexing for the buffer.
int fill_ptr = 0;
int use_ptr =  0;
int count = 0;

//!!!!!Wont need these once done testing!!!!.
int produced = 0;
int consumed = 0;

//Both threads will share for loop counter.
volatile int pcnt = 0;
volatile int ccnt = 0;
volatile int prgFinished = 0;

//This will make sure each thread shares the same matrices for multiplying.
Matrix * M1 = NULL;
Matrix * M2 = NULL;
Matrix * M3 = NULL;

// Define Locks and Condition variables here 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, fill = PTHREAD_COND_INITIALIZER;

// Producer consumer data structures
Matrix * bigmatrix[MAX];

// Bounded buffer put() get()
int put(Matrix * value) {
    // pthread_mutex_unlock(&c_lock);
  *(bigmatrix + fill_ptr) = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
  produced++;
  // printf("PUT, COUNT: %d, THIS IS PRODUCED: %d\n", count, produced);
  return 0;
}

Matrix * get() {
  // Matrix * tmp = *( bigmatrix + (count - 1));
  Matrix * tmp = *( bigmatrix + use_ptr);
  use_ptr = (use_ptr + 1) % MAX;  
  count--;
  consumed++;
  // printf("GET, COUNT: %d, THIS IS CONSUMED: %d\n", count, consumed);
  return tmp;
}

void *prod_worker(void *arg) {

  ProdConsStats * prod_count = (ProdConsStats*) arg;
  //pcnt is a a global variable.
  for (; pcnt < LOOPS; pcnt++) {
    // printf("THIS IS PCNT: %d\n", pcnt);
    pthread_mutex_lock(&lock);
    while (count == MAX && prgFinished == 0) {
        // printf("producer put to SLEEP\n");
        pthread_cond_wait(&empty, &lock); 
    }
    if (prod_count->matrixtotal == LOOPS) {
      // printf("PPP- Should be done!!!!!!\n");
      pthread_cond_broadcast(&fill);
    } else {
        Matrix * M1 = GenMatrixRandom();
        put(M1);
        prod_count->matrixtotal++;
        prod_count->sumtotal += SumMatrix(M1);
        pthread_cond_signal(&fill); 
    }
    pthread_mutex_unlock(&lock); 
  }
    return prod_count;
}

// Matrix CONSUMER worker thread
void *cons_worker(void *arg) {

  ProdConsStats * con_count = (ProdConsStats*) arg;
  //ccnt is a a global variable.
  for (; ccnt < LOOPS; ccnt++) {
    // printf("THIS IS CCNT: %d\n", ccnt);
    pthread_mutex_lock(&lock);
    while (count == 0 && prgFinished == 0) {
        // printf("Consumer put to SLEEP\n");
        pthread_cond_wait(&fill, &lock); 
    }
      //This will grab the first matrix and check if 
      //prg is finished/terminate leftover threads.
      if (M1 == NULL && M2 == NULL && prgFinished != 1) {
        M1 = get();
        con_count->matrixtotal++;
        con_count->sumtotal += SumMatrix(M1);
        // printf("M1 created\n");
        // if (count == 0) {
        //       // printf("M1 Free.\n");
        //       // FreeMatrix(M1);
        //       // M1 = NULL;
        //       sleep(.1);
        //     } 
      //This will grab the second matrix.
     } else if(M1 != NULL && M2 == NULL && prgFinished != 1) {
         M2 = get();
         con_count->matrixtotal++;
         con_count->sumtotal += SumMatrix(M2);
        //  printf("M2 created\n");
         M3 = MatrixMultiply(M1, M2);
         //Check if the multiplication was sucessfull.
         if (M3 != NULL) {
            con_count->multtotal++;
            DisplayMatrix(M1, stdout);
            printf("   X\n");
            DisplayMatrix(M2, stdout);
            printf("   =\n");
            DisplayMatrix(M3, stdout);
            printf("\n");
            FreeMatrix(M1);
            FreeMatrix(M2);
            FreeMatrix(M3);
            M1 = NULL;
            M2 = NULL;
            M3 = NULL;
            // printf("M1 And M2 And M3 Free: \n");
            //Multiplication Failed Free M2.
           } else { 
              FreeMatrix(M2);
              M2 = NULL;
              printf("M2 Free: \n");
              //This will check if we are on last iteration and should delete m1 too.
              if (count == 0) {
                sleep((unsigned int ).1);
                // printf("M1 Free.\n");
                // FreeMatrix(M1);
                // M1 = NULL;
              } 
            }
        }
    //Used if program is over and remaining threads should terminate.
    if (con_count->matrixtotal == LOOPS) {
      if (M1 != NULL) {
        // printf("M1 Free.\n");
        FreeMatrix(M1);
        M1 = NULL;
      }
      // printf("CCC-Should be done!!!!!! %d, %d\n",con_count->matrixtotal, LOOPS );
      prgFinished = 1;
      pthread_cond_broadcast(&empty);
    } else {
       pthread_cond_signal(&empty);
    }
    pthread_mutex_unlock(&lock); 
    // printf("\n");
  }
  return con_count; 
}