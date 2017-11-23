#include "toolbox.h"

typedef struct tool {
  char *name;
  char *usage;
  int (*main)(int, char *[]);
} tool;

tool tools[] = {
  {"echo", "write arguments to the standard output", main_echo},
  {"sh", "command interpreter", main_shell},
  {"ls", "list directory contents", main_ls},
  {"mkdir", "make directories", main_mkdir},
  {"rmdir", "remove empty directories", main_rmdir},
  {"link", "make links between files", main_link},
  {"cat", "concatenate files and print on the standard output", main_cat},
  {"mv", "move (rename) files", main_mv},
  {"cp", "copy files and directories", main_cp},
  {"true", "do nothing, successfully", main_true},
  {"false", "do nothing, unsuccessfully", main_false},
  {NULL, NULL, NULL}
};

int main(int argc, char *argv[]) {
  int i = 0;
  char *name;

  name = basename(argv[1]);
  while ((tools[i].name != NULL) && strcmp(name, tools[i].name)) i++;
  if (tools[i].name != NULL) return tools[i].main(argc-1, argv+1);
  else {
    fprintf(stderr, "usage: tool arg1 ... argN\nwhere tool is a valid tool name among:\n");
    for (i = 0; tools[i].name != NULL; i++)
      fprintf(stderr, "    %s -- %s\n", tools[i].name, tools[i].usage);
    return EXIT_FAILURE;
  }
}

int copy(const char *src, const char *dst, mode_t mode) {
  int fs, fd, length;
  char buf[BUF_SIZE];

  if ((fs = open(src, O_RDONLY)) == -1) {
    perror("open");
    exit(1);
  }
  if ((fd = open(dst, O_CREAT | O_EXCL | O_WRONLY, mode)) == -1) {
    perror("open");
    exit(1);
  }

  while ((length = read(fs, buf, BUF_SIZE)) > 0) {
    if (write(fd, buf, length) == -1) {
      perror("write");
      exit(1);
    }
  };

  if (length    == -1) { perror("read");  exit(1); }
  if (close(fs) == -1) { perror("close"); exit(1); }
  if (close(fd) == -1) { perror("close"); exit(1); }

  return 0;
}
