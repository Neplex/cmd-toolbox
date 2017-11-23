#include "toolbox.h"


int main_mv(int argc, char *argv[]) {
  struct stat buf;

  if (argc != 3) {
    fprintf(stderr, "usage: %s source_file target_file\n", argv[0]);
    exit(2);
  }

  if (rename(argv[1], argv[2]) == -1) {

    if (stat(argv[1], &buf) == -1) {
      perror("stat");
      exit(1);
    }

    copy(argv[1], argv[2], buf.st_mode & 0777);
  }

  return EXIT_SUCCESS;
}
