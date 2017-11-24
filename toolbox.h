#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define BUF_SIZE 4096

int main_echo(int, char *[]);
int main_shell(int, char *[]);

int main_ls(int, char *[]);
int main_mkdir(int, char *[]);
int main_rmdir(int, char *[]);
int main_link(int, char *[]);
int main_cat(int, char *[]);
int main_mv(int, char *[]);
int main_cp(int, char *[]);
int main_rm(int, char *[]);
int main_ln(int, char *[]);

int main_true(int, char *[]);
int main_false(int, char *[]);

int copy(const char*, const char*, mode_t);

#endif /* _TOOLBOX_H_ */
