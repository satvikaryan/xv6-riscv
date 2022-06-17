#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void strcat(char *a, char *b) {
  int i = 0;
  while (a[i] != '\0') i++;
  int j = 0;
  while (b[j] != '\0') {
    a[i] = b[j];
    i++;
    j++;
  }
  a[i] = '\0';
}

char* format_int(int n, char *s, int w) {
  int len = 0;
  while (n > 0) {
    n /= 10;
    len++;
  }
  len = len ? len : 1;
  memset(s, ' ', w-len);
  s[w-len] = '\0';
  return s;
}

char* format_str(char* n, char *s, int w) {
  int len = strlen(n);
  memset(s, ' ', w-len);
  s[w-len] = '\0';
  return s;
}

void ls(char *path) {
  int fd = open(path, 0);
  if (fd < 0) {
    printf("ls: cannot open %s\n", path);
    return;
  }

  struct stat s;
  if (fstat(fd, &s) < 0) {
    printf("ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  char hr[59];
  memset(hr, '-', 58);
  hr[58] = '\0';

  char space0[55];
  char space1[5];
  char space2[10];
  char space3[20];

  printf("%s\n", hr);
  printf("| %s%s |\n", path, format_str(path, space0, 54));
  printf("%s\n", hr);

  if (s.type == T_DIR) {
    struct dirent d;
    while (read(fd, &d, sizeof(d)) == sizeof(d)) {
      if (d.inum == 0) continue;
      char *name = malloc(strlen(path) + strlen(d.name) + 2);
      strcpy(name, path);
      strcat(name, "/");
      strcat(name, d.name);
      if (stat(name, &s) < 0) {
        printf("ls: cannot stat %s\n", name);
        free(name);
        continue;
      }
      if (s.type == T_DIR) {
        printf("| %s%d | %s | %s%d | %s%s |\n", format_int(s.ino, space1, 5), s.ino, "Directory ", format_int(s.size, space2, 10), s.size, d.name, format_str(d.name, space3, 20));
      }
      else if (s.type == T_FILE) {
        printf("| %s%d | %s | %s%d | %s%s |\n", format_int(s.ino, space1, 5), s.ino, "File      ", format_int(s.size, space2, 10), s.size, d.name, format_str(d.name, space3, 20));
      }
      else if (s.type == T_DEVICE) {
        printf("| %s%d | %s | %s%d | %s%s |\n", format_int(s.ino, space1, 5), s.ino, "Device    ", format_int(s.size, space2, 10), s.size, d.name, format_str(d.name, space3, 20));
      }
      else {
        printf("| %s%d | %s | %s%d | %s%s |\n", format_int(s.ino, space1, 5), s.ino, "Unknown   ", format_int(s.size, space2, 10), s.size, d.name, format_str(d.name, space3, 20));
      }
      free(name);
    }
  }
  else if (s.type == T_FILE) {
    printf("| %s%d | %s | %s%d | %s%s |\n", format_int(s.ino, space1, 5), s.ino, "File      ", format_int(s.size, space2, 10), s.size, path, format_str(path, space3, 20));
  }
  else if (s.type == T_DEVICE) {
    printf("| %s%d | %s | %s%d | %s%s |\n", format_int(s.ino, space1, 5), s.ino, "Device    ", format_int(s.size, space2, 10), s.size, path, format_str(path, space3, 20));
  }
  else {
    printf("| %s%d | %s | %s%d | %s%s |\n", format_int(s.ino, space1, 5), s.ino, "Unknown   ", format_int(s.size, space2, 10), s.size, path, format_str(path, space3, 20));
  }

  printf("%s\n", hr);
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    ls(".");
  else
    for (int i = 1; i < argc; i++)
      ls(argv[i]);
  exit(0);
}
