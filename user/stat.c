#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const char* type(int type) {
  switch (type) {
    case T_DIR    : return "Directory ";
    case T_FILE   : return "File      ";
    case T_DEVICE : return "Device    ";
    default       : return "Unknown   ";
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) printf("Usage: %s <pathname>\n", argv[0]);
  struct stat s;
  if (stat(argv[1], &s) < 0) {
    printf("lstat: cannot stat %s\n", argv[1]);
    exit(-1);
  }

  printf("ID of Disk Device:  %d\n", s.dev);
  printf("Type:               %s\n", type(s.type));
  printf("Inode Number:       %d\n", s.ino);
  printf("Number of Links:    %d\n", s.nlink);
  printf("Size:               %d\n", s.size);

  exit(0);
}
