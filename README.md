# xv6 RISC-V Operating System Enhancements

## Overview

This project extends the xv6 RISC-V operating system by adding new system calls to improve functionality. The system calls implemented include process management (`killpid`, `getppid`), message queue management, shared memory, and synchronization mechanisms (semaphores).

## Table of Contents

1. [Introduction](#introduction)
2. [System Calls Added](#system-calls-added)
   - [killpid(pid)](#killpidpid)
   - [getppid()](#getppid)
   - [Message Queue System Calls](#message-queue-system-calls)
   - [Shared Memory System Calls](#shared-memory-system-calls)
   - [Synchronization System Calls](#synchronization-system-calls)
3. [User Programs](#user-programs)
4. [How to Build and Run](#how-to-build-and-run)
5. [Results](#results)

---

## Introduction

The xv6 operating system is a teaching platform for understanding the basics of operating system design. This project enhances the system by introducing advanced process management, inter-process communication (IPC), and synchronization features such as message queues, shared memory, and semaphores.

## System Calls Added

### `killpid(pid)`
Terminates a process identified by its Process ID (PID).

### `getppid()`
Retrieves the parent process ID (PPID) of the calling process.

### Message Queue System Calls

A message queue system is implemented to facilitate communication between processes. The following are the key components:

- `msgqueue`: Represents the queue structure.
  - Fields: `head`, `tail`, `max_msgs`, `curr_msgs`, `max_size`, `refs`, `lock`.
- `msg`: Represents a single message in the queue.
  - Fields: `content`, `size`, `next`.

#### Functions:

- `msgqueue_init`: Initializes the message queue system.
- `sys_msgget`: Creates or retrieves a message queue.
- `sys_msgsend`: Sends a message to the queue.
- `sys_msgrcv`: Receives a message from the queue.
- `sys_msgclose`: Closes a message queue.

### Shared Memory System Calls

This set of system calls implements the management of shared memory regions between processes. The shared memory segments are identified by unique IDs and keys, and their properties (size, reference count, etc.) are managed.

- `shmget`: Creates or accesses a shared memory segment.
- `shmat`: Attaches a shared memory segment to the calling process's address space.
- `shmdt`: Detaches a shared memory segment.
- `shmctl`: Performs control operations on shared memory segments (e.g., remove segment).

### Synchronization System Calls

Semaphores are used for process synchronization. The following system calls are added:

- `seminit`: Initializes the semaphore system.
- `sem_create`: Creates a semaphore.
- `sem_wait`: Waits (blocks) on a semaphore.
- `sem_signal`: Signals a semaphore, unblocking any waiting processes.
- `sem_delete`: Deletes a semaphore.

### `readcount` System Call

The `readcount` system call is designed to track the number of times the `read()` system call is invoked by a process. This is useful for debugging, logging, or monitoring the activity of processes that perform read operations.

## User Programs

The following user programs demonstrate the usage of the new system calls:

- `user_killpid.c`: Demonstrates the `killpid` system call.
- `user_getppid.c`: Retrieves and displays the parent process ID using `getppid`.
- `user_msgqueue.c`: Implements operations on message queues (`msgget`, `msgsend`, `msgrcv`, `msgclose`).
- `user_writershm.c` and `user_readershm.c`: Demonstrate shared memory usage (`shmget`, `shmat`, `shmdt`).
- `user_sem_test.c`: Tests semaphore-based synchronization (`sem_create`, `sem_wait`, `sem_signal`).
- `user_producer_consumer.c`: Implements a solution to the producer-consumer problem using semaphores.
- `user_readcount1.c`: Demonstrates the `readcount` system call, tracking read system calls.
- `user_readcount2.c`: Similar to `readcount1`, but uses pipes for communication.

## How to Build and Run

### Building xv6

Clone the repository and build xv6 with the following commands:


```bash
git clone <repo-url>
cd xv6-riscv
make clean
make qemu
```

### Executing User Programs

Once xv6 is running, you can execute the user programs as follows:

```bash
user_killpid    # Demonstrates process termination
user_getppid    # Retrieves the parent process ID
user_msgqueue   # Demonstrates message queue operations
user_writershm  # Writes to shared memory
user_readershm  # Reads from shared memory
user_sem_test   # Tests semaphore synchronization
user_producer_consumer  # Producer-consumer problem solution
user_readcount1 # Tracks read system calls
user_readcount2 # Tracks read system calls using pipes
```

