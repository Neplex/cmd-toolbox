#include "toolbox.h"

int main_ln(int argc, char *argv[]) {

    if(argc == 4){
      if(!strcmp(argv[3],"-s")) {
        if (symlink(argv[1], argv[2]) == -1) {
          perror("symlink");
          exit(1);
        }
      }
    } else if (argc == 3) {
      if (link(argv[1], argv[2]) == -1) {
        perror("link");
        exit(1);
      }
    } else {
      fprintf(stderr, "usage: %s source_file target_file\n", argv[0]);
      exit(2);
    }
    return EXIT_SUCCESS;
}
