#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 5
#define MAX_LOOPS 30

char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];

int count;
void flushlog();

struct _args
{
   unsigned int interval;
};

sem_t readers;
sem_t writers;

pthread_mutex_t mtx;

void *wrlog(void *data) // writer
{
   sem_wait(&writers);
   pthread_mutex_lock(&mtx);
   char str[MAX_LOG_LENGTH];
   int id = *(int*) data;
   usleep(20);
   sprintf(str,"%d", id);
   strcpy(logbuf[count], str);
   /* Only increase count to size MAX_BUFFER_SLOT*/
   count = (count > MAX_BUFFER_SLOT) ? count : count + 1;
   if(count == MAX_BUFFER_SLOT) sem_post(&readers);
   pthread_mutex_unlock(&mtx);
   return 0;
}

void flushlog() // reader
{
   int i;
   char nullval[MAX_LOG_LENGTH];
   // mutex_lock
   sem_wait(&readers); 
   pthread_mutex_lock(&mtx);
   for (i = 0; i < count; i++)
   {
      sprintf(nullval, "%d", -1);
      printf("Slot  %i: %s\n", i, logbuf[i]);
      strcpy(logbuf[i], nullval);
   }
   fflush(stdout);
   count = 0;
   pthread_mutex_unlock(&mtx);
   while(i--){
      sem_post(&writers);
   }
   return;
}

void *timer_start(void *args)
{
   while (1)
   {
      flushlog();
      /*Waiting until the next timeout */
      usleep(((struct _args *) args)->interval);
   }
}

int main()
{
   int i;
   count = 0;
   pthread_t tid[MAX_LOOPS];
   pthread_t lgrid;

   sem_init(&writers, 0,MAX_BUFFER_SLOT);
   sem_init(&readers, 0,0);
   pthread_mutex_init(&mtx,NULL);


   int id[MAX_LOOPS];

   struct _args args;
   args.interval = 500e3;
   /*500 msec ~ 500 * 1000 usec */

   /*Setup periodically invoke flushlog() */
   pthread_create(&lgrid, NULL, &timer_start, (void*) &args);

   /*Asynchronous invoke task writelog */
   for (i = 0; i < MAX_LOOPS; i++)
   {
      id[i] = i;
      pthread_create(&tid[i], NULL, wrlog, (void*) &id[i]);
   }

   for (i = 0; i < MAX_LOOPS; i++)
      pthread_join(tid[i], NULL);

   sleep(2);

   return 0;
}
