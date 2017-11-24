#include "toolbox.h"

int main_rm(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usage: %s directory_name\n", argv[0]);
    exit(2);
  }

  if (remove(argv[1]) == -1) {
    perror("rmdir");
    exit(1);
  }

  return EXIT_SUCCESS;
}
