#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int main(int argc, char* argv[]) {
  sleep(atoi(argv[1]));
  exit(0);
}
