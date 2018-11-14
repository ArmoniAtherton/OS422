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
int use_ptr =  0;
int count = 0;
int produced = 0;
int consumed = 0;

//Both threads will share for loop counter.
volatile int pcnt = 0;
volatile int ccnt = 0;

// Define Locks and Condition variables here 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, fill = PTHREAD_COND_INITIALIZER;

// Producer consumer data structures
Matrix * bigmatrix[MAX];


// Bounded buffer put() get()
int put(Matrix * value)
{
    // pthread_mutex_unlock(&c_lock);
  *(bigmatrix + fill_ptr) = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
  
  produced++;
  printf("PUT, COUNT: %d, THIS IS PRODUCED: %d\n", count, produced);
  // printf("THIS IS PRODUCED: %d\n", produced);
  return NULL;
}

Matrix * get() 
{
  // Matrix * tmp = *( bigmatrix + (count - 1));
  Matrix * tmp = *( bigmatrix + use_ptr);
  use_ptr = (use_ptr + 1) % MAX;  
  count--;
  consumed++;
  printf("GET, COUNT: %d, THIS IS CONSUMED: %d\n", count, consumed);
  // printf("THIS IS CONSUMED: %d\n", consumed);
  return tmp;
}

void *prod_worker(void *arg)
{
  ProdConsStats * prod_count = (ProdConsStats*) arg;
  // int i = 0;
  for (; pcnt < LOOPS; pcnt++) {
    
    printf("THIS IS PCNT: %d\n", pcnt);
    pthread_mutex_lock(&lock);
    while (count == MAX) {
        pthread_cond_wait(&empty, &lock); 
    }
    put(GenMatrixRandom());
    if (pcnt < LOOPS) {
    // pthread_mutex_lock(&c_lock);
    prod_count->matrixtotal++;
    // pthread_mutex_unlock(&c_lock);
    }
    pthread_cond_signal(&fill); 
    pthread_mutex_unlock(&lock); 
    
  }
    // printf("PRODUCED: %d\n", produced);
    return prod_count;
}

// Matrix CONSUMER worker thread
void *cons_worker(void *arg)
{
  ProdConsStats * con_count = (ProdConsStats*) arg;
  // int i = 0;
  Matrix * M1 = NULL;
  Matrix * M2 = NULL;
  Matrix * M3 = NULL;
  for (; ccnt < LOOPS; ccnt++) {
    printf("THIS IS CCNT: %d\n", ccnt);
    pthread_mutex_lock(&lock);
    while (count == 0) {
        pthread_cond_wait(&fill, &lock); 
    }
     //This will grab the first matrix.
      if (M1 == NULL && M2 == NULL) {
        M1 = get();
        if (ccnt < LOOPS) {
        // pthread_mutex_lock(&c_lock);
        con_count->matrixtotal++;
        // pthread_mutex_unlock(&c_lock);
        }
        printf("M1 created\n");
        if (count == 0) {
              printf("M1 Free.\n");
              FreeMatrix(M1);
              M1 = NULL;
            }
      //This will grab the second matrix.
     } else if(M1 != NULL && M2 == NULL) {
         M2 = get();
        if (ccnt < LOOPS) {
        //  pthread_mutex_lock(&c_lock);
         con_count->matrixtotal++;
        //  pthread_mutex_unlock(&c_lock);
        }
         printf("M2 created\n");
         M3 = MatrixMultiply(M1, M2);
         //Check if the multiplication was sucessfull.
         if (M3 != NULL) {
            if (ccnt < LOOPS) {
            // pthread_mutex_lock(&c_lock);
            con_count->multtotal++;
            // pthread_mutex_unlock(&c_lock);
            }
            DisplayMatrix(M3, stdout);
            FreeMatrix(M1);
            FreeMatrix(M2);
            FreeMatrix(M3);
            M1 = NULL;
            M2 = NULL;
            M3 = NULL;
             printf("M1 And M2 And M3 Free: \n");
             //Multiplication Failed Free M2.
           } else { 
              FreeMatrix(M2);
              M2 = NULL;
              printf("M2 Free: \n");
              //This will check if we are on last iteration and should delete m1 too.
              if (count == 0) {
                printf("M1 Free.\n");
                FreeMatrix(M1);
                M1 = NULL;
              } 
            }
        }
      
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&lock); 
    printf("\n");
  }
  return con_count; 
}

  /** Testing version **/
  // if (M1 == NULL && M2 == NULL) {
  //      M1 = get();
  //      con_count->matrixtotal++;
  //      //Check if on last iteration.
  //     //  if (count == 0) {
  //     //    FreeMatrix(M1);
  //     //    M1 = NULL;
  //     //    printf("M1 is now null\n");
  //     //  } else {
  //     //      printf("M1 created\n");
  //     //  }

  //     //This will grab the second matrix.
  //    } else if(M1 != NULL && M2 == NULL) {
  //        M2 = get();
  //        con_count->matrixtotal++;
  //        printf("M2 created\n");
     
  //        M3 = MatrixMultiply(M1, M2);
  //        if (M3 != NULL) {
  //           con_count->multtotal++;
  //           DisplayMatrix(M3, stdout);
  //           FreeMatrix(M1);
  //           FreeMatrix(M3);
  //           M1 = NULL;
  //           M3 = NULL;
  //          }
  //           FreeMatrix(M2);
  //           M2 = NULL;
  //           printf("M2 deleted.\n");
  //          }
      
  //     if (M1 != NULL && count == 0) {
  //         printf("M1 deleted.\n");
  //         FreeMatrix(M1);
  //         M1 = NULL;
  //     }
  //     // if (count == 0) {
  //    pthread_cond_signal(&empty);