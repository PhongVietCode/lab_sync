#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (pnum + 4 ) % N
#define RIGHT (pnum + 1) % N

int phil_state[N];
int philosophies[N] = {0,1,2,3,4};

sem_t mutex;
sem_t sem_phil_state[N];

void *philosopher(void*);
int main(){
    int i;
    pthread_t thread_id[N];

    sem_init(&mutex, 0, 1);

    for(i = 0 ;i < N;i++)
        sem_init(&sem_phil_state[i], 0,0);
    for(i= 0; i < N;i++){
        printf("Philosopher %d enter the room\n", i + 1);
        pthread_create(&thread_id[i], NULL, philosopher, &philosophies[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
}
void test_phil(int pnum){
    if( (phil_state[pnum] == HUNGRY) &&
        (phil_state[LEFT] != EATING)&&
        (phil_state[RIGHT] != EATING))
    {
        phil_state[pnum] = EATING;
        sleep(2);
        printf("Philosopher %d takes fork %d and %d\n",
                pnum + 1, LEFT + 1, pnum + 1);
        printf("Philosopher %d is eating\n", pnum + 1);

        sem_post(&sem_phil_state[pnum]);
    }
}
void take_fork(int pnum){
    if(phil_state[pnum] != HUNGRY){
        sem_wait(&mutex);
        phil_state[pnum] = HUNGRY;

        printf("Philosopher %d is Hungry\n", pnum + 1);

        test_phil(pnum);
        sem_post(&mutex);

        // if test success , sem_wait will be pass, else it will wait until it it invoke by another philosophy
        sem_wait(&sem_phil_state[pnum]);
        sleep(1);
    }
}
void put_fork(int pnum){
    sem_wait(&mutex);
    if(phil_state[pnum] == EATING){
        phil_state[pnum] = THINKING;
        printf("Philosopher %d putting fork %d and %d down\n",
            pnum + 1, LEFT + 1, pnum + 1);
        printf("Philosopher %d is thinking\n", pnum + 1);

        // invoke the 2 next if they are hungry or not
        test_phil(LEFT);
        test_phil(RIGHT);
    }
    
    sem_post(&mutex);
}

void *philosopher(void *num){
    while(1){
        int *i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}
