/*
SNEH SUMAN
2019337
OS A4
SUBMITTED USING THE LATE POLICY*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

typedef pthread_mutex_t Mutex; 
typedef pthread_cond_t Cond;
//defining the MACRO for easier use in program 
#define LEFT (n + 4) % 5
#define RIGHT (n + 1) % 5
#define BOWL 2
void starter(int n);
void *t=NULL;
//creating my semaphore 
//Using a structure data type
typedef struct {
  int value, wakeups;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond;
} MySemaphore;


MySemaphore *make_semaphore(int value);
void semaphore_wait(MySemaphore *semaphore);
void semaphore_signal(MySemaphore *semaphore);
int r=0;

//A function to check if memory can be allocated or not
void * check_malloc(size_t num_bytes) {
  void *p = malloc(num_bytes);

  if(p == t) {
   
    printf("Error, couldn't allocate memory.\n");
    exit(-1);
  }
   //free(p);
  return p;
}
//Implementing my own mutex trylock for non-blocking functionality implementation   
void mutex_trylock(Mutex *mutex)
 {
 int n=pthread_mutex_trylock(mutex);
  if(n!=r) perror("lock failed"); 
 } 
//creatiing a makemutex() function to create a mutex
Mutex* make_mutex(void)
 { 
  int n; Mutex* mutex=(Mutex*)malloc(sizeof(Mutex)); 
  n=pthread_mutex_init(mutex,t); 
  if(n!=r) perror("make_lock failed"); 
  //free(mutex);
  return(mutex);
   }

//implementing my own mutex_unlock 
void mutex_unlock(Mutex *mutex) 
{ 
  int n=pthread_mutex_unlock(mutex); 
  if(n!=r) perror("unlock failed");
  }
  //made a condition checker
 Cond* make_cond(void) 
 {
  int n;
   Cond* cond=(Cond*)malloc(sizeof(Cond));
    n=pthread_cond_init(cond,t);
    if(n!=r) perror("Make_cond failed"); 
   // free(cond);
    return(cond);
     } 
     void cond_wait(Cond* cond,Mutex* mutex) 
     { 
      int n=pthread_cond_wait(cond,mutex); 
      if(n!=r) perror("cond_wait failed"); 
    } 
    void cond_signal(Cond* cond) 
    { 
      int n=pthread_cond_signal(cond);
    if(n!=r) perror("cond_signal failed");
    }
    int value=0;
//A function to create semaphore
MySemaphore *make_semaphore(int value)
{
  MySemaphore *semaphore = check_malloc(sizeof(MySemaphore));
  semaphore->mutex = make_mutex();
  semaphore->cond = make_cond();
  semaphore->value = value;
  semaphore->wakeups = r;

  return semaphore;
}
//Implemented my own wait()
void semaphore_wait(MySemaphore *semaphore)
{
  pthread_mutex_trylock(semaphore->mutex);
  value=value-1;
  semaphore->value;

  if (semaphore->value < r) {
    do 
    {
      pthread_cond_wait(semaphore->cond, semaphore->mutex);
    } while (semaphore->wakeups < r+1);
    semaphore->wakeups--;
  }
  pthread_mutex_unlock(semaphore->mutex);
}
int semset;
//Implemented my own signal()
void semaphore_signal(MySemaphore *semaphore)
{
  pthread_mutex_trylock(semaphore->mutex);
  semaphore->value++;

  if (semaphore->value <= r) 
  {
    semaphore->wakeups++;
    pthread_cond_signal(semaphore->cond);
  }
  pthread_mutex_unlock(semaphore->mutex);
}
//Defind a funtion for debugging by printing the semaphore values
int signal_Printprimitive(int semnr,int value)
{
  int erg;
  erg = semctl(semset, semnr, GETVAL, value);
  return erg;
}

    

    pthread_t philosopher[5];
    pthread_mutex_t forks[5];

    
    void Nap() {
        usleep(random() % 20000);
    }

    // Philosopher tries to grab the first fork
    int LockLowestFork(int n) {
        if ( n < ((n+1)%5) ) {
            return pthread_mutex_trylock(&forks[n]);
        }
        else {
            if (n > (RIGHT)) {
                return pthread_mutex_trylock( & forks[RIGHT]);
            } else {
                if (1) {
                    printf("error");
                }
            }
        }
        return -1;
    }

    // Philosopher tries to grab the second fork
    int LockHigherFork(int n) {
        if ( n > RIGHT ) {
            return pthread_mutex_trylock(&forks[n]);
        }
        else {
            if (n < (RIGHT)) {
                return pthread_mutex_trylock( & forks[RIGHT]);
            } else {
                if (1) {
                    printf("error");
                }
            }
        }
        return -1;
    }

    // The forks are put down by the philosopher
    void Eat(int n) {
        pthread_mutex_unlock(&forks[n]);
        pthread_mutex_unlock(&forks[RIGHT]);
    }

    int main() {
        int i;

        // Initialize the philosopher threads
        i = 0;
        while (i < 5) {
            pthread_mutex_init(&forks[i],NULL);
            i++;
        }
        // Create the philosopher threads
        for (i = 0; i < 5; i++) {
            pthread_create(&philosopher[i], NULL, (void *) starter,(int *)(intptr_t) i);
        }
        // Do not exit program until all threads are finished.
        i = 0;
        while (i < 5) {
            pthread_join(philosopher[i], NULL);
            i++;
        }
        for (i = 0; i < 5; i++) {
            pthread_mutex_destroy(&forks[i]);
        }

     

        return 0;
    }
    //The main processing of the solution is done in this function 
    void starter(int n) {
        long int x = 5000000000;
        // Flag that determines if the philosopher needs to try to get a fork again
        bool failed = true;
        do {
            do {
                // if the philosopher failed to get the fork, nap and try again
                // otherwise, continue (failed is now false, so exit loop)
                if (LockLowestFork(n) != 0) {
                    Nap();
                }
                else {
                    failed = false;
                }
            } while (failed);

            //Reinitialize the variable to true
            failed = true;
            if (1) {
                printf("Philosopher %ld got first fork%d\n\n",pthread_self(),LEFT );
            }
            
            do {
               //Check if the philosopher is able to grab the second fork
                if (LockHigherFork(n) != 0) {
                    
                    Nap();
                }
                else {
                  //printf("Philosopher %ld failed to get the second fork\n",pthread_self());
                 
                    failed = false;
                }
            } while (failed);

            //reinitialize the variable to true
            failed = true;

            if (1) {
                printf("Philosopher %ld got second fork%d\n",pthread_self(),RIGHT );
            }
            
            if (1) {
                printf("Philosopher %ld is done\n",pthread_self());
            }
            Nap();
            if (1) {
                
            }
            Eat(n);
            Nap();

            x--;
        } while (x > r);

    }

