#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NPAGES 256
#define PGSIZE 4096
#define MINLEN 4
#define MAXLEN 12

int
isalnumch(char c)
{
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int
ishexonly(char *s, int len)
{
  for (int i = 0; i < len; i++) {
    char c = s[i];
    int ishex = (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
    if (!ishex)
      return 0;
  }
  return 1;
}

int
iscommonword(char *s, int len)
{
  static char *words[] = {
    "secret", "attack", "usage", "null", "fork", "runcmd", "exec",
    "failed", "open", "pipe", "missing", "file", "redirection",
    "syntax", "many", "args", "parseblock", "leftovers", "console",
    "cannot", 0
  };
  for (int wi = 0; words[wi]; wi++) {
    int wl = strlen(words[wi]);
    if (wl != len)
      continue;
    int match = 1;
    for (int j = 0; j < len; j++) {
      if (s[j] != words[wi][j]) {
        match = 0;
        break;
      }
    }
    if (match)
      return 1;
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  char *mem = sbrk(NPAGES * PGSIZE);
  int i = 0;

  while (i < NPAGES * PGSIZE) {
    if (isalnumch(mem[i])) {
      int start = i;
      while (i < NPAGES * PGSIZE && isalnumch(mem[i]))
        i++;
      int len = i - start;
      if (len >= MINLEN && len <= MAXLEN &&
          !ishexonly(mem + start, len) &&
          !iscommonword(mem + start, len)) {
        write(1, mem + start, len);
        write(1, "\n", 1);
      }
    } else {
      i++;
    }
  }

  exit(0);
}