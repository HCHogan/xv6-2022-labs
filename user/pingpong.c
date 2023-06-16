#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int p[2];
int pid;
char buf[16];

int main(int argc, char* argv[]) {
  pipe(p);
  if((fork()) == 0) {
    read(p[0], buf, 5);
    printf("%d: received %s\n", getpid(), buf);
    write(p[1], "pong", 5);
    close(p[1]);
    close(p[0]);
  } else {
    write(p[1], "ping", 5);
    read(p[0], buf, 5);
    printf("%d: received %s\n", getpid(), buf);
    close(p[1]);
    close(p[0]);
  }
  exit(0);
}
