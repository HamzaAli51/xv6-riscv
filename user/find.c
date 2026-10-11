#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "user/user.h"

void
find(char *path, char *name, int exec_mode, int exec_argc, char *exec_argv[])
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type == T_FILE){
    p = path + strlen(path);

    while(p > path && *(p - 1) != '/')
      p--;

    if(strcmp(p, name) == 0){

      if(!exec_mode){
        printf("%s\n", path);
      }
      else {
        int pid;
        char *args[MAXARG];
        int i;

        for(i = 0; i < exec_argc; i++)
          args[i] = exec_argv[i];

        args[exec_argc] = path;
        args[exec_argc + 1] = 0;

        pid = fork();

        if(pid < 0){
          fprintf(2, "find: fork failed\n");
        }
        else if(pid == 0){
          exec(args[0], args);
          fprintf(2, "find: exec %s failed\n", args[0]);
          exit(1);
        }
        else {
          wait(0);
        }
      }
    }

    close(fd);
    return;
  }

  if(st.type != T_DIR){
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "find: path too long\n");
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);

  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    if(strcmp(de.name, ".") == 0 ||
       strcmp(de.name, "..") == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = '\0';

    find(buf, name, exec_mode, exec_argc, exec_argv);
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  int exec_mode = 0;
  int exec_argc = 0;
  char *exec_argv[MAXARG];
  int i;

  if(argc < 3){
    fprintf(2, "Usage: find path name [-exec cmd ...]\n");
    exit(1);
  }

  if(argc > 3){

    if(strcmp(argv[3], "-exec") != 0){
      fprintf(2, "Usage: find path name [-exec cmd ...]\n");
      exit(1);
    }

    exec_mode = 1;

    for(i = 4; i < argc; i++){
      if(exec_argc >= MAXARG - 1){
        fprintf(2, "find: too many arguments\n");
        exit(1);
      }

      exec_argv[exec_argc] = argv[i];
      exec_argc++;
    }

    if(exec_argc == 0){
      fprintf(2, "find: missing command\n");
      exit(1);
    }
  }

  find(argv[1], argv[2], exec_mode, exec_argc, exec_argv);

  exit(0);
}
