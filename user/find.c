#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int flagSuc = 0;

void find(char* dirname, char* filename) {
  char buf[512];
  int fd;
  struct stat st;
  struct dirent de;
  if((fd = open(dirname, 0)) < 0) {
    printf("find: cannot open %s\n", dirname);
    return;
  }

  if(fstat(fd, &st) < 0) {
    printf("find: cannot stat\n");
    close(fd);
    return;
  }

  if(strlen(dirname) + 1 + DIRSIZ + 1 > sizeof buf) {
    fprintf(2, "find: path too long\n");
    exit(0);
  }

  strcpy(buf, dirname);
  char* p = buf + strlen(dirname);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if(de.inum == 0) continue;
    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    stat(buf, &st);
    switch(st.type) {
    case T_FILE:
      if(strcmp(de.name, filename) == 0) {
        flagSuc = 1;
        printf("%s\n", buf);
      }
      break;
    case T_DIR:
      find(buf, filename);
      break;
    }
  }
  close(fd);

}

int main(int argc, char* argv[]) {
  if(argc > 3 || argc < 2) {
    printf("usage: %s <dire> <filename>\n", argv[0]);
    exit(0);
  }
  if(argc == 2) {
    find(".", argv[1]);
  } else {
    find(argv[1], argv[2]);
  }

  exit(0);
}
