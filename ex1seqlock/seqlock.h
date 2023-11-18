#ifndef PTHREAD_H
#include <pthread.h>
#include <semaphore.h>
#include<stdio.h>
#endif

// critical section allow many readers but only 1 writer
// writer can access critical section when there is no reader
// reader can access critical section when there is no writer
typedef struct pthread_seqlock { /* TODO: implement the structure */
   pthread_cond_t db; // for writer and reader
   pthread_mutex_t mutex; // for readers
   int writer_count;
   int reader_count;
   int sequence;
   int writer_waiting;
} pthread_seqlock_t;

static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_init(&rw->mutex, NULL);
   pthread_cond_init(&rw->db, NULL);
   rw->reader_count = 0;
   rw->writer_count = 0;
   rw->sequence = 0;
   rw->writer_waiting = 0;
}
// static inline void pthread_rwlock_destroy(pthread_seqlock_t *rw ){
//    pthread_mutex_destroy(&rw->mutex);
//    pthread_cond_destroy(&rw->writers);
//    pthread_cond_destroy(&rw->readers);
//    rw->data = 0;
//    rw->sequence = 0;
//    rw->writer_waiting = 0;
// }
// Writer lock
static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mutex);
   rw->writer_waiting = 1;
   while (rw->writer_count > 0) {
      pthread_cond_wait(&rw->db, &rw->mutex);
   }
   rw->sequence++;
   rw->writer_waiting = 0;
   rw->writer_count++;
   printf("Writer in \n ");
   pthread_mutex_unlock(&rw->mutex);
}
// Writer unlock
static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mutex);
   rw->writer_count--;
   rw->sequence++;
   printf("Writer out \n");
   pthread_cond_broadcast(&rw->db); // like signal
   pthread_mutex_unlock(&rw->mutex);
}
// Reader lock
static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mutex);
   if(rw->reader_count == 0){
      while (rw->writer_count > 0) {
         pthread_cond_wait(&rw->db, &rw->mutex);
      }
   }
   while (rw->sequence % 2 != 0 || rw->writer_waiting > 0){
      pthread_cond_wait(&rw->db, &rw->mutex);
   }
   rw->reader_count++;
   printf("Reader in \n");
   pthread_mutex_unlock(&rw->mutex);
   return 1;
}

// Reader unlock
static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mutex);
   rw->reader_count--;
   printf("Reader out \n");
   if(rw->reader_count == 0)    
      pthread_cond_broadcast(&rw->db); // like signal
   pthread_mutex_unlock(&rw->mutex);
   return 0;
}

