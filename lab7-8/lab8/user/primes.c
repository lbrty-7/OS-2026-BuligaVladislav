#include "kernel/types.h"
#include "user/user.h"

// one stage of the sieve: reads numbers from 'left', prints the first one
// (a prime p) and forwards to the next stage only the numbers not divisible by p
void sieve(int left) __attribute__((noreturn));

void
sieve(int left)
{
  int p, n;
  if(read(left, &p, sizeof(p)) != sizeof(p)){   // nothing left: last stage
    close(left);
    exit(0);
  }
  printf("prime %d\n", p);

  int right[2];
  pipe(right);
  if(fork() == 0){          // the next stage
    close(right[1]);
    close(left);
    sieve(right[0]);
  }
  close(right[0]);
  while(read(left, &n, sizeof(n)) == sizeof(n)){
    if(n % p != 0)
      write(right[1], &n, sizeof(n));
  }
  close(left);
  close(right[1]);          // the next stage sees end of file
  wait(0);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if(fork() == 0){
    close(p[1]);
    sieve(p[0]);
  }
  close(p[0]);
  for(int i = 2; i <= 35; i++)
    write(p[1], &i, sizeof(i));
  close(p[1]);
  wait(0);
  exit(0);
}
