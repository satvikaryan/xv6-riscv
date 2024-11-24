#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_


struct msg {
  char* content;
  int size;
  struct msg* next;
};

struct msgqueue {
  struct msg* head;
  struct msg* tail;
  int max_msgs;    // Maximum number of messages in queue
  int curr_msgs;   // Current number of messages
  int max_size;    // Maximum size of each message
  int refs;        // Number of processes referencing this queue
  struct spinlock lock;
};

#define MAX_QUEUES 16
#define MAX_MSG_SIZE 256
#define MAX_MSGS_PER_QUEUE 32

#endif