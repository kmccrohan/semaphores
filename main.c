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
#include <semaphore.h>


const char* FULL_NAME = "/depalmaFull";
const char* EMPTY_NAME = "/depalmaFull";
const char* LOCK_NAME = "/depalmaMutexLock";

sem_t* mutexFull;
sem_t* mutexEmpty;
sem_t* mutexLock;

// Constants
#define PROD 1
#define CONS 0


/**
Executes critical sectiono of both consumer and producer.
*/
void criticalSection(int who) {
  if (who == PROD)
    printf("I am the producer producing.\n");
  else
    printf("I am the consumer consuming.\n");
}

/**
Produces and changes semaphores accordingly.
*/
void producer() {
  for(int i = 0; i < 5; i++){
    sem_wait(mutexEmpty);
    sem_wait(mutexLock);
    criticalSection(PROD);
    sem_post(mutexLock);
    sem_post(mutexFull);
  }
}

/**
Consumes and changes semaphores accordingly.
*/
void consumer() {

  for (int i=0; i < 10; i++) {
    sem_wait(mutexFull);
    sem_wait(mutexLock);
    criticalSection(CONS);
    sem_post(mutexLock);
    sem_post(mutexEmpty);
  }
  
  exit(0);
}

/**
Creates semaphores with specified initial value in empty semaphore.
*/
void createSemaphores(int initialValueFull){
  mutexFull = sem_open(FULL_NAME, O_CREAT, S_IRWXG, 0);
  mutexEmpty = sem_open(EMPTY_NAME, O_CREAT, S_IRWXG, initialValueFull);
  mutexLock = sem_open(LOCK_NAME, O_CREAT, S_IRWXG, 1);
}

/**
Closes semaphores.
*/
void closeSemaphores(){
  sem_close(mutexFull);
  sem_close(mutexEmpty);
  sem_close(mutexLock);
}

int main(int argc, char** argv) {
  createSemaphores(100);
  int value;
  if ((value = fork()) < 0)
    printf("Child could not be created.\n");
  else
    if (value == 0)
      producer();
    else
      consumer();
  closeSemaphores();
}
