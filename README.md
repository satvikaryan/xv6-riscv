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
   - **Implementation**: 
     - Traverses the process table to find the process with the given PID.
   - **Key Features**:
     - Prevents termination of non-existent processes by returning an error.
     - Ensures all resources of the process are freed before termination.

### 2. **`getppid()`**
   - **Purpose**: Retrieves the parent process ID (PPID) of the calling process.
   - **Implementation**: 
     - Accesses the process structure of the calling process to return its parent's PID.
   - **Key Features**:
     - Helps trace the hierarchy of processes.
     - Simplifies debugging and inter-process relationship tracking.

---

## Demonstration of Features

### User Program: `killpid.c`

To validate the implementation of `killpid` and `getppid`, we developed the `killpid.c` program, which demonstrates both functionalities.


## How to Build and Run

### 1. **Build xv6**
   Clone the repository and navigate to the xv6 directory:
   ```bash
   git clone <repo-url>
   cd xv6
   make qemu
   ```

### 2. **Run the User Program**
   - Execute the `killpid` program:
     ```bash
     killpid
     ```

---

## Screenshots

### Output of `killpid.c`

#### 1. Parent Process:
   - Displays its PPID and the PID of the child.
   - Terminates the child process after a brief delay.
   
#### 2. Child Process:
   - Prints its PID and is terminated.

![Killpid Output](images/killpid_output.png)

---

