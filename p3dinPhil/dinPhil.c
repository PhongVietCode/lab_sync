#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

pthread_mutex_t mtx;
pthread_mutex_t mtx2;

pthread_cond_t chopstick[N];
int chopstick_status[N];
void *philosopher(void*);
void eat(int);
void think(int);
int main()
{
   int i, a[N];
   pthread_t tid[N];

   pthread_mutex_init(&mtx, NULL);
   pthread_mutex_init(&mtx2, NULL);

   for (i = 0; i < N; i++){
	   pthread_cond_init(&chopstick[i], NULL);
      chopstick_status[i] = 0;
   }

   for (i = 0; i < 5; i++)
   {
      a[i] = i;
      pthread_create(&tid[i], NULL, philosopher, (void*) &a[i]);
   }

   for (i = 0; i < 5; i++)
      pthread_join(tid[i], NULL);
}

void *philosopher(void *num)
{
   int phil = *(int*) num;
   printf("Philosopher %d has entered room\n", phil);
	  
   while (1)
   {
      pthread_mutex_lock(&mtx);

      while (chopstick_status[phil] || chopstick_status[(phil + 1) % N]) {
         pthread_cond_wait(&chopstick[phil], &mtx);
      }

      chopstick_status[phil] = 1;
      chopstick_status[(phil + 1) % N] = 1;

      pthread_mutex_unlock(&mtx);
      printf("Philosopher %d takes fork %d and %d\n",
	          phil, phil, (phil + 1) % N);
			  
      eat(phil);
      sleep(2);

      printf("Philosopher %d puts fork %d and %d down\n",
	          phil, (phil + 1) % N, phil);
      pthread_mutex_lock(&mtx);
      chopstick_status[phil] = 0;
      chopstick_status[(phil + 1) % N] = 0;
      pthread_cond_signal(&chopstick[phil]);
      pthread_cond_signal(&chopstick[(phil + 1) % N]);
      pthread_mutex_unlock(&mtx);
      think(phil);
      sleep(1);
   }
}

void eat(int phil)
{
   printf("Philosopher %d is eating\n", phil);
}

void think(int phil)
{
   printf("Philosopher %d is thinking\n", phil);
}