# xv6 RISC Operating System Enhancements

This repository contains modifications to the xv6 RISC-V operating system as part of our project. We have added new functionalities through system calls to enhance process management and signaling mechanisms.

---

## Project Overview

This project extends the xv6 operating system by:
1. Analyzing the existing implementation of system calls.
2. Adding new system calls for advanced process management.
3. Demonstrating their usage through a combined user program.
4. Documenting the design, implementation, and results.

---

## Added System Calls

### 1. **`killpid(pid)`**
   - **Purpose**: Terminates a process with the specified PID.

### 2. **`getppid()`**
   - **Purpose**: Retrieves the parent process ID (PPID) of the calling process.

### 3. **`msgqueue`**
   - **Purpose**: Manages inter-process communication using message queues.

### 4. **`writershm`**
   - **Purpose**: Writes data into shared memory.

### 5. **`readershm`**
   - **Purpose**: Reads data from shared memory.

### 6. **`readcount1`**
   - **Purpose**: Tracks and retrieves the read count for a specific shared resource.

### 7. **`readcount2`**
   - **Purpose**: Similar to `readcount1` .

### 8. **`sem_test`**
   - **Purpose**: Tests semaphore-based synchronization between processes.

### 9. **`producer_consumer`**
   - **Purpose**: Implements the producer-consumer problem using messag and sye queue and synchronization mechanisms.


## Demonstration of Features

### User Program: `killpid.c`

To validate the implementation of `killpid` and `getppid`, we developed the `killpid.c` program, which demonstrates both functionalities.


## How to Build and Run

### 1. **Build xv6**
   Clone the repository and navigate to the xv6 directory:
   ```bash
   git clone <repo-url>
   cd xv6-riscv
   make clean
   make qemu
   ```

# User Programs Execution Guide

   - Execute the `killpid` program:
     ```bash
     user_killpid
     ```

   - **Get Parent Process ID**:
     ```bash
     user_getppid
     ```

   - **Message Queue Program**:
     ```bash
     user_msgqueue
     ```

   - **Writer Shared Memory Program**:
     ```bash
     user_writershm
     ```

   - **Reader Shared Memory Program**:
     ```bash
     user_readershm
     ```

   - **Read Count Program 1**:
     ```bash
     user_readcount1
     ```

   - **Read Count Program 2**:
     ```bash
     user_readcount2
     ```

   - **Semaphore Test Program**:
     ```bash
     user_sem_test
     ```

   - **Producer-Consumer Program**:
     ```bash
     user_producer_consumer
     ```


## Notes:
- Ensure that the environment variable `USER_OBJ_DIR` is set to the correct directory where the user programs are located.
- Each program can be executed individually using the respective command listed above.



---

## Screenshots

![Killpid Output](images/killpid_output.png)

---

