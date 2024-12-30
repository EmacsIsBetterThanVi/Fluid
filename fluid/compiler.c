#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <setjmp.h>
#ifndef _WIN32
#include <unistd.h>
#endif
extern int main(int argc, char ** argv);
int compile(FILE * inf, char * outf, bool verbose, char * mainFunc){
  char * ProgramContent = malloc(ftell(inf));
}
