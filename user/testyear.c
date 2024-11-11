#include "user/user.h"
#include "kernel/types.h"

int main() {
    int year = getyear();
    printf("The year is: %d\n", year);
    exit(0);
}
