#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MEMORY_KEY 1000  
#define MEMORY_SIZE 2048 

int main()
{
    int memory_id;
    char *shared_data;
    char user_input[200];

  
    memory_id = shmget(MEMORY_KEY, MEMORY_SIZE, 0); 
    if (memory_id < 0)
    {
        printf("Error: Failed to create or access shared memory.\n");
        exit(1);
    }
    printf("Shared memory created or accessed successfully. Memory ID: %d\n", memory_id);

    
    shared_data = (char *)shmat(memory_id);
    if (shared_data == (char *)-1)
    {
        printf("Error: Could not attach to shared memory.\n");
        exit(1);
    }
    printf("Attached to shared memory segment.\n");

   
    printf("Enter a message to store in shared memory: ");
    gets(user_input, sizeof(user_input)); 

    int index = 0;
    while (user_input[index] != '\0' && index < MEMORY_SIZE - 1)
    {
        shared_data[index] = user_input[index];
        index++;
    }
    shared_data[index] = '#'; 

    if (shmdt(memory_id) < 0)
    {
        printf("Error: Failed to detach from shared memory.\n");
        exit(1);
    }
    printf("Message written to shared memory successfully: %s\n", user_input);

    exit(0);
}
