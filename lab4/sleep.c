#include "kernel/types.h"
#include "user/user.h"

// sleep: pause for a given number of clock ticks.
// In current xv6-riscv the sleep() system call was renamed to pause().

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);
  }
  pause(atoi(argv[1]));   // system call into the kernel
  exit(0);
}
