#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    printf("Salaam %s! Kaifa Haluka?\n", argv[1]);
  } else {
    printf("Salaam Robo! Kaifa Haluka?\n");
  }
  exit(0);
}
