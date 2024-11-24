#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "proc.h"
#include "semaphore.h"
// #include "spinlock.h"

struct {
  struct spinlock lock;
  struct semaphore sems[MAX_SEMAPHORES];
} stable;

void
seminit(void)
{
  initlock(&stable.lock, "semaphore");
  for(int i = 0; i < MAX_SEMAPHORES; i++) {
    stable.sems[i].inuse = 0;
    initlock(&stable.sems[i].lock, "sem");
  }
}

int
sem_create(char *name, int value)
{
  struct semaphore *s;
  int i;

  if(name == 0 || strlen(name) >= SEM_NAME_LEN)
    return -1;

  acquire(&stable.lock);

  // Check if semaphore already exists
  for(i = 0; i < MAX_SEMAPHORES; i++) {
    s = &stable.sems[i];
    if(s->inuse && strncmp(s->name, name, SEM_NAME_LEN) == 0) {
      release(&stable.lock);
      return i;
    }
  }

  // Find free slot
  for(i = 0; i < MAX_SEMAPHORES; i++) {
    s = &stable.sems[i];
    if(!s->inuse) {
      s->inuse = 1;
      s->value = value;
      s->nwaiting = 0;
      safestrcpy(s->name, name, SEM_NAME_LEN);
      release(&stable.lock);
      return i;
    }
  }

  release(&stable.lock);
  return -1;
}

void
sem_wait(int sem_id)
{
  struct semaphore *s;
  struct proc *p = myproc();

  if(sem_id < 0 || sem_id >= MAX_SEMAPHORES)
    return;

  s = &stable.sems[sem_id];
  acquire(&s->lock);

  while(s->value <= 0) {
    if(s->nwaiting < NPROC) {
      s->waiting[s->nwaiting++] = p;
      sleep(p, &s->lock);  // Releases lock while sleeping
    }
    // Re-acquire lock after wakeup
    if(!s->inuse) {
      release(&s->lock);
      return;
    }
  }

  s->value--;
  release(&s->lock);
}

void
sem_signal(int sem_id)
{
  struct semaphore *s;
  struct proc *p;

  if(sem_id < 0 || sem_id >= MAX_SEMAPHORES)
    return;

  s = &stable.sems[sem_id];
  acquire(&s->lock);

  s->value++;

  if(s->nwaiting > 0) {
    p = s->waiting[--s->nwaiting];
    wakeup(p);
  }

  release(&s->lock);
}

int
sem_delete(int sem_id)
{
  struct semaphore *s;

  if(sem_id < 0 || sem_id >= MAX_SEMAPHORES)
    return -1;

  s = &stable.sems[sem_id];
  acquire(&s->lock);

  if(!s->inuse) {
    release(&s->lock);
    return -1;
  }

  while(s->nwaiting > 0) {
    wakeup(s->waiting[--s->nwaiting]);
  }

  s->inuse = 0;
  release(&s->lock);
  return 0;
}

