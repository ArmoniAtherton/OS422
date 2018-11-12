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
int use_ptr =  1 - MAX;
int count = 0;
int produced = 0;
int consumed = 0;

// Define Locks and Condition variables here 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, fill = PTHREAD_COND_INITIALIZER;

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
  Matrix * tmp = *( bigmatrix + (count -1));
  count--;
  printf("GET, COUNT: %d\n", count);
  //  printf("Use Ptr, COUNT: %d\n", use_ptr);
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
  Matrix * M1 = NULL;
  Matrix * M2 = NULL;
  Matrix * M3 = NULL;
  for (i = 0; i < LOOPS; i++) {
    pthread_mutex_lock(&lock);
    while (count == 0) {
        pthread_cond_wait(&fill, &lock); 
    }
      //Grab our first matrix
      if (M1 == NULL && M2 == NULL) {
        M1 = get();
        //Check if on last iteration.
        if (count == 0) {
          M1 = NULL;
          printf("M1 is now null\n");
        } else {
            printf("M1 created\n");
        }
       
       //This will grab the second matrix.
      } else if(M1 != NULL && M2 == NULL) {
        // This will check if only one is left in the buffer.
          // if (count == 0) {
          //     M1 = NULL;
          //     printf("Signal to produce more.\n");
          //     pthread_cond_signal(&empty); 
          // } else {
              M2 = get();
              printf("M2 created\n");
          // }
          //This will multiply the matrices
      } else {
          M3 = MatrixMultiply(M1, M2);
          if (M3 == NULL) {
            M2 = NULL;
            printf("Multiplication FAILED:\n");
          } else {
            M1 = NULL;
            M2 = NULL;
            DisplayMatrix(M3, stdout);
            M3 = NULL;
          }
      }
    // Matrix * tmp1 = get();

    
    // printf("THIS is Produced: %d, Consumed: %d, %d\n", produced, consumed, (produced - consumed)); 
    // printf("SIGNAL TO PRODUCE: \n");
    pthread_cond_signal(&empty); 
    pthread_mutex_unlock(&lock); 

    //Print the matrix.
    // DisplayMatrix(tmp1, stdout);
    // FreeMatrix(tmp1);
    printf("\n");
  }
  return NULL; 
}

//  Matrix * tmp1 = get();
//     //If there is only one matrix in buffer and We have produced all matricies possible.
//     if (count == 0 && produced == MAX) {
//       FreeMatrix(tmp1);

//     }  //Will check if we can keep producing more matricies but one one matrix in buffer.
//     else if (count == 0 && produced != MAX) {

//     }  //Matricies are in the buffer and we keep grabing and multiplying.
//     else {
        
//     }
// Matrix * tmp2 = get();
    // Matrix * tmp3 = MatrixMultiply(tmp1, tmp2);
    //This will check if they multiplied correctly.
    // while(tmp3 == NULL) {
    //   FreeMatrix(tmp2);
    //   FreeMatrix(tmp3);
    //   tmp2 = get();
    //   tmp3 = MatrixMultiply(tmp1, tmp2);
    //   printf("IN THE LOOP\n");
    // }
  