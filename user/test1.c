#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int read_count;

    // Call the system call to get the read count
    read_count = getreadcount();

    // Print the result returned by the system call
    printf("Read count: %d\n", read_count);

    exit(1);
}
