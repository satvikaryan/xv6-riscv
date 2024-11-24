#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "spinlock.h"
#include "param.h"
#define MAX_SEMAPHORES 32
#define SEM_NAME_LEN   16

struct semaphore {
  int value;                    // Current semaphore value
  struct spinlock lock;         // Lock for atomic operations
  struct proc *waiting[NPROC];  // Array of waiting processes
  int nwaiting;                // Number of waiting processes
  int inuse;                   // Whether this semaphore slot is in use
  char name[SEM_NAME_LEN];     // Name of the semaphore
};

int  sem_create(char *name, int value);
void sem_wait(int sem_id);
void sem_signal(int sem_id);
int  sem_delete(int sem_id);

// Initialize the semaphore system
void seminit(void);

#endif