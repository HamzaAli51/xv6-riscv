#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "Usage: %s secret\n", argv[0]);
    exit(1);
  }

  char *mem = sbrk(10 * PGSIZE);
  char *p = mem + 5 * PGSIZE;

  int i = 0;
  while(argv[1][i] != 0){
    p[i] = argv[1][i];
    i++;
  }
  p[i] = 0;

  exit(0);
}