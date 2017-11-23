#include "toolbox.h"

#define isr(x) "-r"[x >> 2 & 1]
#define isw(x) "-w"[x >> 1 & 1]
#define isx(x) "-x"[x & 1]
#define rwx(x) isr(x), isw(x), isx(x)
#define rights(x) rwx(x>>6), rwx(x>>3), rwx(x)
#define filetype(x) "?pc?d?b?-?l?s???"[x >> 12 & 017]

int main_ls(int argc, char *argv[]) {

  DIR *dir;
  struct dirent *dirent;
  struct stat buf;
  struct tm lt;
  struct group *grp;
  struct passwd *pwd;
  char date[16];
  char lname[NAME_MAX+1];

  dir = opendir(argc>1?argv[1]:".");

  while ((dirent = readdir(dir))) {
    if (lstat(dirent->d_name, &buf) == -1) {
      perror("stat");
      exit(1);
    }

    pwd = getpwuid(buf.st_uid); // USER
    grp = getgrgid(buf.st_gid); // GROUP
    localtime_r(&buf.st_mtime, &lt);
    strftime(date, sizeof(date), "%d %b %H:%M", &lt); // TIME

    printf("%ld\t%c%c%c%c%c%c%c%c%c%c %ld %s\t%s\t%ld\t%s %s", dirent->d_ino,
           filetype(buf.st_mode), rights(buf.st_mode), buf.st_nlink, pwd->pw_name,
           grp->gr_name, buf.st_size, date, dirent->d_name);

    if (dirent->d_type == DT_LNK) {
      int  x = readlink(dirent->d_name, lname, sizeof(lname));
      lname[x]='\0';
      printf(" -> %s", lname);
    }

    printf("\n");
  }

  closedir(dir);

  return 0;
}
