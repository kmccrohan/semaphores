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
  criticalSection(PROD);
}

/**
Consumes and changes semaphores accordingly.
*/
void consumer() {
  criticalSection(CONS);
}

int main(int argc, char** argv) {

  int value;
  if ((value = fork()) < 0)
    printf("Child could not be created.\n");
  else
    if (value == 0)
      producer();
    else
      consumer();
}
