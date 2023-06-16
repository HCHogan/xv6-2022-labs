#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int p[2];
int buf, flag, prime;

void handle(int fd[]) {
  close(fd[1]);
  int p_next[2];
  pipe(p_next);
  flag = read(fd[0], &buf, 4);
  if(flag > 0) {
    printf("prime %d\n", buf);
    prime = buf;
    if(fork() == 0) {
      close(fd[0]);
      handle(p_next);
    } else {
      while(read(fd[0], &buf, 4) > 0) {
        if(buf % prime != 0) {
          write(p_next[1], &buf, 4);
        }
      }
      close(p_next[1]);
      close(fd[0]);
      wait(0);
      exit(0);
    }
  } else {
    /* if read falied(last process), release and exit */
    close(fd[0]);
    close(p_next[0]);
    close(p_next[1]);
    exit(0);
  }
}

int main(int argc, char *argv[]) {

  pipe(p);

  if((fork()) == 0) {
    handle(p);
  } else {
    close(p[0]);
    for(int i = 2; i <= 35; ++i) {
      write(p[1], &i, 4);
    }
    close(p[1]);
  }

  wait(0);
  exit(0);
}
