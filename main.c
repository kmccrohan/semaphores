/**
Producer consumer problem.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>


const char* FULL_NAME = "/depalmaFull";
const char* EMPTY_NAME = "/depalmaFull";

sem_t* mutexFull;
sem_t* mutexEmpty;

// Constants
#define PROD 1
#define CONS 0

void producer() {
}

void consumer() {
}

void criticalSection(int who) {
}

void createSemaphores(int intialValueFull){
  mutexFull = sem_open(FULL_NAME, O_CREAT, S_IRWXG, intialValueFull);
  mutexEmpty = sem_open(EMPTY_NAME, O_CREAT, S_IRWXG, 0);
}

void closeSemaphores(){
  sem_close(mutexFull);
  sem_close(mutexEmpty);
}

int main() {
  createSemaphores(100);
  closeSemaphores();
}
