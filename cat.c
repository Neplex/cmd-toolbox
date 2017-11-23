#include "toolbox.h"

int main_cat(int argc, char *argv[]) {
  int i;
  char buf[1024];
  FILE* file;

  for (i = 1; i < argc; i++)
    if((file = fopen(argv[i], "r"))) {
      while (fgets(buf, 1024, file)) puts(buf);
      fclose(file);
    } else {
      printf("%s : No such file\n", argv[i]);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
