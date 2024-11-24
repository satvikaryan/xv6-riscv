#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "shm.h"
#include "semaphore.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_retint()
{
  return 2024;
}


//return the year of which
//Unix version 6 was released

uint64 
sys_getyear(void) 
{

return 1975;
}

uint64
sys_getppid(void) {
    struct proc *p = myproc();
    return p->parent ? p->parent->pid : 0;  // Return 0 if no parent exists
}

// System call: shmget
uint64 
sys_shmget(void) 
{
    int key, size, flags;
    argint(0, &key);
    argint(1, &size);
    argint(2, &flags);
    return shmget(key, size, flags);
}

// System call: shmat
uint64 
sys_shmat(void) 
{
    int shmid;
    argint(0, &shmid);
    return shmat(shmid);
}

// System call: shmdt
uint64 
sys_shmdt(void) 
{
    int shmid;
    argint(0, &shmid);

    return shmdt(shmid);
}

// System call: shmctl
uint64 
sys_shmctl(void) 
{
    int shmid, cmd;
    argint(0, &shmid);
    argint(1, &cmd);

    return shmctl(shmid, cmd);
}
int
sys_getreadcount(void)
{
  return myproc()->readid;
}

// System call implementations
uint64
sys_sem_create(void)
{
  char name[64];
  int value;

  if(argstr(0, name, sizeof(name)) < 0)
    return -1;
  argint(1, &value);

  return sem_create(name, value);
}

uint64
sys_sem_wait(void)
{
  int sem_id;

  argint(0, &sem_id);

  sem_wait(sem_id);
  return 0;
}

uint64
sys_sem_signal(void)
{
  int sem_id;

  argint(0, &sem_id);

  sem_signal(sem_id);
  return 0;
}

uint64
sys_sem_delete(void)
{
  int sem_id;

  argint(0, &sem_id);

  return sem_delete(sem_id);
}
