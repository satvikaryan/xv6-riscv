// #include "kernel/types.h"
#include "user/user.h"
// #include "kernel/stat.h"
int main() {
    int year = getyear();
    printf("The year is: %d\n", year);
    exit(0);
}
