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
// int use_ptr =  1 - MAX;
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
  
  produced++;
  printf("PUT, COUNT: %d, THIS IS PRODUCED: %d\n", count, produced);
  // printf("THIS IS PRODUCED: %d\n", produced);
  return NULL;
}

Matrix * get() 
{
  Matrix * tmp = *( bigmatrix + (count - 1));
  count--;
  consumed++;
  printf("GET, COUNT: %d, THIS IS CONSUMED: %d\n", count, consumed);
  // printf("THIS IS CONSUMED: %d\n", consumed);
  return tmp;
}

void *prod_worker(void *arg)
{
  ProdConsStats * prod_count = (ProdConsStats*) arg;
  int i = 0;
  for (i = 0; i < LOOPS; i++) {
    pthread_mutex_lock(&lock);
    while (count == MAX) {
        pthread_cond_wait(&empty, &lock); 
    }
    put(GenMatrixRandom());
    prod_count->matrixtotal++;
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
  int i = 0;
  Matrix * M1 = NULL;
  Matrix * M2 = NULL;
  Matrix * M3 = NULL;
  for (i = 0; i < LOOPS; i++) {
    pthread_mutex_lock(&lock);
    while (count == 0) {
        pthread_cond_wait(&fill, &lock); 
    }
      if (M1 == NULL && M2 == NULL) {
       M1 = get();
       con_count->matrixtotal++;
       //Check if on last iteration.
       if (count == 0) {
         FreeMatrix(M1);
         M1 = NULL;
         printf("M1 is now null\n");
       } else {
           printf("M1 created\n");
       }

      //This will grab the second matrix.
     } else if(M1 != NULL && M2 == NULL) {
         M2 = get();
         con_count->matrixtotal++;
         printf("M2 created\n");
     
         M3 = MatrixMultiply(M1, M2);
         if (M3 != NULL) {
            con_count->multtotal++;
            DisplayMatrix(M3, stdout);
            FreeMatrix(M1);
            FreeMatrix(M3);
            M1 = NULL;
            M3 = NULL;
           }
            FreeMatrix(M2);
            M2 = NULL;
           }
      
      if (count == 0 && M1 != NULL) {
          printf("M1 deleted.\n");
          FreeMatrix(M1);
          M1 = NULL;
      }
      if (count == 0) {
           pthread_cond_signal(&empty);
      }
    pthread_mutex_unlock(&lock); 
    printf("\n");
    
  }
  return con_count; 
}

//  // printf("THIS IS THE LOOP BEFORE ENTERING: %d\n", i);
//       //Grab our first matrix
//       if (M1 == NULL && M2 == NULL) {
//         M1 = get();
//         con_count->matrixtotal++;
//         //Check if on last iteration.
//         if (count == 0) {
//           FreeMatrix(M1);
//           M1 = NULL;
//           printf("M1 is now null\n");
//         } else {
//             printf("M1 created\n");
//         }
       
//        //This will grab the second matrix.
//       } else if(M1 != NULL && M2 == NULL) {
//           M2 = get();
//           con_count->matrixtotal++;
//           // if (count == 0) {
//           //   FreeMatrix(M1);
//           //   FreeMatrix(M2);
//           //   M1 = NULL;
//           //   M2 = NULL;
//           //   printf("M1/M2 is now null\n");
//           //   } else {
//                 printf("M2 created\n"); 
//                  printf("1.) M1: %d, M2: %d\n",M1, M2);
//                 M3 = MatrixMultiply(M1, M2);
//                 printf("2.) M1: %d, M2: %d\n",M1, M2);
//                 if (M3 == NULL) {
//                   printf("M2: %d\n", M2);
//                   FreeMatrix(M2);
//                   M2 = NULL;
//                   // printf("********%d\n", M2);
//                   printf("Multiplication FAILED:\n");
//                 } else {
//                     printf("PRINTED CORRECTLY: M1- %d, M2- %d,  M3- %d", M1, M2, M3);
//                     con_count->multtotal++;
//                     FreeMatrix(M1);
//                     FreeMatrix(M2);
//                     M1 = NULL;
//                     M2 = NULL;
//                     DisplayMatrix(M3, stdout);
//                     FreeMatrix(M3);
//                     M3 = NULL;
//                 }
//             // }
//       } 
  
//     // printf("SIGNAL TO PRODUCE: \n");
//     pthread_cond_signal(&empty); 
//     pthread_mutex_unlock(&lock); 

//     //Print the matrix.
//     // DisplayMatrix(tmp1, stdout);
//     // FreeMatrix(tmp1);
//     printf("\n");
//   }
//   printf("CONSUMED: %d, PRODUCED: %d\n", consumed, produced);
//   return con_count; 
  


    // //This will grab the first matrix.
    //   if (M1 == NULL && M2 == NULL) {
    //     M1 = get();
    //     con_count->matrixtotal++;
    //     printf("M1 created\n");
    //     if (count == 0) {
    //           printf("M1 Free.\n");
    //           FreeMatrix(M1);
    //           M1 = NULL;
    //         }

    //   //This will grab the second matrix.
    //  } else if(M1 != NULL && M2 == NULL) {
    //      M2 = get();
    //      con_count->matrixtotal++;
    //      printf("M2 created\n");
    //      M3 = MatrixMultiply(M1, M2);
    //      //Check if the multiplication was sucessfull.
    //      if (M3 != NULL) {
    //         con_count->multtotal++;
    //         DisplayMatrix(M3, stdout);
    //         FreeMatrix(M1);
    //         FreeMatrix(M2);
    //         FreeMatrix(M3);
    //         M1 = NULL;
    //         M2 = NULL;
    //         M3 = NULL;
    //          printf("M1 And M2 And M3 Free: \n");
    //          //Multiplication Failed Free M2.
    //        } else { 
    //           FreeMatrix(M2);
    //           M2 = NULL;
    //           printf("M2 Free: \n");
    //           //This will check if we are on last iteration and should delete m1 too.
    //           if (count == 0) {
    //             printf("M1 Free.\n");
    //             FreeMatrix(M1);
    //             M1 = NULL;
    //           }
    //         }
            
    //     }
      
    //   //Should only signal if count is zero. If check not here signal early.
    //   if (count == 0) {
    //        pthread_cond_signal(&empty);
    //   }