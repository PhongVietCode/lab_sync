#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "seqlock.h"  /* TODO implement this header file */

pthread_seqlock_t lock;
sem_t mu;
long val = 0;
void * writer(void *arg) {
   pthread_seqlock_wrlock(&lock);
   for(int i = 0;i < 10000;i++){
      val++;
   }
   printf("Writer: %ld\n", val);
   pthread_seqlock_wrunlock(&lock);
   pthread_exit(NULL);
}

void * reader(void * arg) {
  if(pthread_seqlock_rdlock(&lock)){
      printf("Reader val = %ld\n", val); 
      pthread_seqlock_rdunlock(&lock);
   }
  pthread_exit(NULL);
}
int main()
{
   // int val = 0;
   pthread_seqlock_init(&lock);
   sem_init(&mu,0,1);
   
   // pthread_seqlock_wrlock(&lock);
   // val++;
   // pthread_seqlock_wrunlock(&lock);

   // for(int i = 0;i < 10;i++){
   //    if(pthread_seqlock_rdlock(&lock) == 1){
   //       printf("val = %d\n", val); 
   //       pthread_seqlock_rdunlock(&lock);
   //    }
   // }
   int threads = 10;
   int i=0;
   pthread_t writers[threads];
   pthread_t readers[threads];
   int tid[threads];


   for (i = 0; i < threads; i++) {
      tid[i] = i;
      // Create producer thread
      pthread_create( &writers[i], NULL, writer, (void * ) tid[i]);

      // Create consumer thread
      
   }
   for (i = 0; i < threads; i++) {
      tid[i] = i;
      // Create producer thread
      // pthread_create( &writers[i], NULL, writer, (void * ) tid[i]);
   pthread_create( &readers[i], NULL, reader, (void * ) tid[i]);

      // Create consumer thread
      
   }

   for (i = 0; i < threads; i++) {
      pthread_join(writers[i], NULL);
      pthread_join(readers[i], NULL);
   }

   printf("End val: %ld\n", val);

}
