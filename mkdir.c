#include "toolbox.h"

int main_mkdir(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usage: %s directory_name\n", argv[0]);
    exit(2);
  }

  if (mkdir(argv[1], 0777) == -1) {
    perror("mkdir");
    exit(1);
  }

  return 0;
}
