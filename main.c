/**
Project 6
CPSC 346, Sec 02
Kyle McCrohan (kmccrohan@zagmail.gonzaga.edu)
Ethan Mahintorabi (emahintorabi@zagmail.gonzaga.edu)

Description:
- forks child and parent process. One becomes producer and one consumer.
- producers and consumers do their things, respectively, and set semaphores accordingly.
- a mutex protects critical section.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define SEMKEY 0 
typedef struct sembuf sem_buf;
sem_buf semWait[1], semSignal[1];

int semFull, semEmpty, semLock;

// Constants
#define PROD 1
#define CONS 0

void printSemValues() 
{
  int m, e, f; 
  m = semctl(semLock, 0, GETVAL, 0);
  e = semctl(semEmpty, 0, GETVAL, 0);
  f = semctl(semFull, 0, GETVAL, 0);
  printf("Lock: %d empty: %d full: %d\n", m,e,f);

}

/**
Executes critical sectiono of both consumer and producer.
*/
void criticalSection(int who) 
{
  if (who == PROD)
    printf("I am the producer producing.\n");
  else
    printf("I am the consumer consuming.\n");
}

/**
Produces and changes semaphores accordingly.
*/
void producer() {
  
  for(int i = 0; i < 5; i++)
  {
    semop(semEmpty, semWait, 1);
    semop(semLock, semWait, 1);
    criticalSection(PROD);
    semop(semLock, semSignal, 1);
    semop(semFull, semSignal, 1);
  }
}

/**
Consumes and changes semaphores accordingly.
*/
void consumer() 
{
  
  for (int i=0; i < 5; i++) 
  {
    semop(semFull, semWait, 1);
    semop(semLock, semWait, 1);
    criticalSection(CONS);
    semop(semLock, semSignal, 1);
    semop(semEmpty, semSignal, 1);
    sleep(1);
  }
  
  exit(0);
}

/**
Creates semaphores with specified initial value in empty semaphore.
*/
void createSemaphores(int initialValueFull){

  //set the buffer values for wait.
  semWait[0].sem_num = 0;
  semWait[0].sem_op = -1; //decrement
  semWait[0].sem_flg = SEM_UNDO;   

  //set the buffer values for signal 
  semSignal[0].sem_num = 0;
  semSignal[0].sem_op = 1; //increment
  semSignal[0].sem_flg = SEM_UNDO;

  short int in[1];
  in[0] = 1;

  //create the mutex semaphore.
  semLock = semget(SEMKEY, 1, 0777 | IPC_CREAT);
  semctl(semLock, 1 , SETALL, in);

  // Create empty and full semaphores, set initial values accordingly.
  in[0] = 0;
  semFull = semget(SEMKEY, 1, 0777 | IPC_CREAT);
  semctl(semFull, 1 , SETALL, in);

  in[0] = initialValueFull;
  semEmpty = semget(SEMKEY, 1, 0777 | IPC_CREAT);
  semctl(semEmpty, 1 , SETALL, in);
}

/**
Closes semaphores.
*/
void closeSemaphores(){
  semctl(semFull, 1, IPC_RMID, NULL);
  semctl(semEmpty, 1, IPC_RMID, NULL);
  semctl(semLock, 1, IPC_RMID, NULL);
}

/**
Main
*/
int main(int argc, char** argv) {

  createSemaphores(100);
  
  int value;
  if ((value = fork()) < 0)
    printf("Child could not be created.\n");
  else
  {
    if (value == 0)
      producer();
    else
      consumer();
    closeSemaphores();
  }
}
