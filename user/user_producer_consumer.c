#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_MSG_SIZE 256
#define NUM_ITEMS 5
void producer(int qid, int sem_print) {
    char msg[MAX_MSG_SIZE];
    
    for(int i = 0; i < NUM_ITEMS; i++) {
        // Create the message with item number
        msg[0] = '0' + i;  // Convert number to character
        msg[1] = '\0';     // Null terminate
        
        // Wait for printing permission
        sem_wait(sem_print);
        
        // Send the message
        if(msgsend(qid, msg, strlen(msg)) < 0) {
            printf("Producer: Failed to send item %d\n", i);
            exit(1);
        }
        
        printf("Producer: Sent item %d\n", i);
        sleep(10);  // Simulate some work
        
        // Allow consumer to print
        sem_signal(sem_print);
        
        // Add small delay to ensure consumer gets chance to acquire semaphore
        sleep(5);
    }
    
    exit(0);
}

void consumer(int qid, int sem_print) {
    char msg[MAX_MSG_SIZE];
    
    for(int i = 0; i < NUM_ITEMS; i++) {
        // Wait for producer to finish printing
        sem_wait(sem_print);
        
        // Receive message
        int n = msgrcv(qid, msg, sizeof(msg));
        if(n < 0) {
            printf("Consumer: Failed to receive item %d\n", i);
            exit(1);
        }
        
        // Null terminate the message
        msg[n] = 0;
        // Convert character back to number
        int item = msg[0] - '0';
        printf("Consumer: Received item %d\n", item);
        sleep(20);  // Simulate some work
        
        // Allow producer to print
        sem_signal(sem_print);
        
        // Add small delay to ensure producer gets chance to acquire semaphore
        sleep(5);
    }
    
    exit(0);
}

int main(void) {
    int qid, pid;
    int sem_print;
    
    // Create message queue
    qid = msgget();
    if(qid < 0) {
        printf("Failed to create message queue\n");
        exit(1);
    }
    
    // Create semaphore for print synchronization
    sem_print = sem_create("print", 1);
    if(sem_print < 0) {
        printf("Failed to create semaphore\n");
        exit(1);
    }
    
    printf("Starting producer-consumer test with message queues...\n");
    
    // Create producer process
    pid = fork();
    if(pid < 0) {
        printf("Fork failed for producer\n");
        exit(1);
    }
    
    if(pid == 0) {  // Child process (Producer)
        producer(qid, sem_print);
    }
    
    // Create consumer process
    pid = fork();
    if(pid < 0) {
        printf("Fork failed for consumer\n");
        exit(1);
    }
    
    if(pid == 0) {  // Child process (Consumer)
        consumer(qid, sem_print);
    }
    
    // Parent process waits for both children
    wait(0);  // Wait for producer
    wait(0);  // Wait for consumer
    
    // Clean up
    msgclose(qid);
    sem_delete(sem_print);
    
    printf("Producer-consumer test completed!\n");
    exit(0);
}