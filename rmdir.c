#include "toolbox.h"

int main_rmdir(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usage: %s directory_name\n", argv[0]);
    exit(2);
  }

  if (rmdir(argv[1]) == -1) {
    perror("rmdir");
    exit(1);
  }

  return 0;
}
