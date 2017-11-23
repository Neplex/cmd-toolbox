#include "toolbox.h"

int fdpipe;

int chdir_cmd(char *argv[]) {
  if (argv[1] != NULL && argv[2] == NULL) {
    return chdir(argv[1]);
  } else {
    fprintf(stderr, "usage: cd directory\n");
    return 1;
  }
}

struct internalcmd {
  char *name;
  int (*cmd)(char *[]);
};

struct internalcmd internals[] = {
  {"cd", chdir_cmd},
  {NULL, NULL}
};

/* EX simple_cmd */
int redir_cmd(char *argv[], int in, int out) {
  pid_t blop;
  int st, i;

  /*printf("EXEC :");
  for (int i=0; argv[i]; i++) printf("%s, ", argv[i]);
  printf("\n");*/

  if (!argv[0]) {
    fprintf(stderr, "empty command...\n");
    return 0;
  }
  for (i = 0; internals[i].name; i++)
    if (strcmp(argv[0], internals[i].name) == 0)
      return internals[i].cmd(argv);
  blop = fork();
  if (blop == -1) {
    perror("fork");
    return 1;
  }

  if (blop) {
    waitpid(blop, &st, 0);
    return WEXITSTATUS(st);
  } else {
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    if (in>STDERR_FILENO)  close(in);
    if (out>STDERR_FILENO) close(out);
    execvp(argv[0], argv);
    perror(argv[0]);
    exit(1);
  }

  return 0;
}

int start_cmd(char *argv[], int in) {
  int fd[2], status;
  pipe(fd);
  status = redir_cmd(argv, in, fd[1]);
  fdpipe = fd[0];
  close(fd[1]);
  return status;
}

int next_cmd(char *argv[]) {
  int fd[2], status;
  pipe(fd);
  status = redir_cmd(argv, fdpipe, fd[1]);
  close(fdpipe);
  fdpipe = fd[0];
  close(fd[1]);
  return status;
}

int last_cmd(char *argv[], int out) {
  int status;
  status = redir_cmd(argv, fdpipe, out);
  close(fdpipe);
  return status;
}

char *strip(char *s) {
  char *cur;

  if ((cur=strpbrk(s, "#"))) *cur='\0';
  cur=s+strlen(s)-1;
  if (*cur=='\n') cur--;
  while ((cur>=s) && (*cur==' ')) cur--;
  *(cur+1)='\0';
  for(cur=s;*cur==' ';cur++);

  return cur;
}

int parse_line(char *s) {
  char *cur, *argv[1024];
  int i, env = 0;

  i = 0;
  argv[i++] = cur = strip(s);
  while ((cur = strpbrk(cur, " "))) {
    *cur++ = '\0';
    while (*cur == ' ') cur++;
    argv[i++] = cur;
  }
  argv[i] = NULL;

  if (strcmp("exit", argv[0]) == 0) exit(0);

  /* ENV */
  for (i = 0; argv[i]; i++) {
    if (argv[i][0] == '$' && !(argv[i] = getenv(argv[i]+1))) argv[i] = "";
    if (strstr(argv[i], "=")) {
      char* key = strtok(argv[i], "=");
      char* val = strtok(NULL, "=");
      setenv(key, val, 1);
      argv[i] = val;
      if(i==0) env=1;
    }
  }

  char* in = NULL;
  char* out = NULL;
  char** start = argv;

  /* Redirections */
  for (i = 0; argv[i]; i++) {
    if (argv[i][0] == '<') {
      argv[i] = NULL;
      start = &argv[i+1];
      if (i>0) in = argv[i-1];
    } else
    if (argv[i][0] == '>') {
      argv[i] = NULL;
      out = argv[i+1];
    }
  }

  int fdin = 0, fdout = 1;
  if (in) fdin  = open(in, O_RDONLY);
  if (out) fdout = open(out, O_CREAT | O_TRUNC | O_WRONLY, 0666);

  /* Filter */
  int prev = 0, status = 0;
  char** next = start;

  while (*start && !env) {
    while (*next && (*next)[0]!='|') next++;

    if (*next) {
      *next = NULL;
      if (prev) next_cmd(start);
      else { start_cmd(start, fdin); prev=1; }
      next++;
    } else {
      status = prev?last_cmd(start, fdout):redir_cmd(start, fdin, fdout);
    }

    start = next;
  }

  return status;
}

void read_script(char *filename) {
  char buf[1024];
  FILE* file;

  if((file = fopen(filename, "r"))) {
    while (fgets(buf, 1024, file)) {
      parse_line(buf);
    }
    fclose(file);
  } else printf("%s : No such file\n", filename);

  exit(1);
}

int main_shell(int argc, char *argv[]) {
  char buf[1024];
  int status;

  if (argv[1]) {
    read_script(argv[1]);
  } else
    while (1) {
      printf("$ ");
      fflush(stdout);
      status = parse_line(fgets(buf, 1024, stdin));
      if (status) printf("exit %d\n", status);
    }

  return EXIT_SUCCESS;
}
