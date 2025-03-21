#ifndef PIPE_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PREAD 1
#define PWRITE 2
#define PREADWRITE 2
int * openP(){
  int * PIPE = (int *)calloc(2, sizeof(int));
  pipe(PIPE);
  return PIPE;
}
int writeP(int PIPE[2], const char * buffer){
  return write(PIPE[0], (void *)buffer, strlen(buffer)+1);
}
char * readP(int PIPE[2], int size){
  char * tmp = (char *)malloc(size);
  read(PIPE[1], tmp, size);
  return tmp;
}
int invokeP(int (*f)(int), int MODE, const char * prg, const char ** args){
  int * PIPE = openP();
  int C;
  if((C=fork()) == 0){
    if((MODE & 2) == 2) dup2(PIPE[1], 1);
    if((MODE & 1) == 1) dup2(PIPE[0], 0);
    close(PIPE[1]);
    close(PIPE[2]);
    execlp(prg, prg, args);
  } else{
    if((MODE & 2) == 2) dup2(PIPE[1], 0);
    if((MODE & 1) == 1) dup2(PIPE[0], 1);
    close(PIPE[1]);
    close(PIPE[0]);
    return f(C);
  }
  return 0;
}
#define PIPE_H
#endif
