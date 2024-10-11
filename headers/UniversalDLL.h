#ifndef UDLL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
jmp_buf LIBLOAD_FAILED;
jmp_buf FUNCLOAD_FAILED;
#ifdef _WIN32
#error "Why are you using windows it is a bad OS. Switch to Linux if you want to us this header"
#include <windows.h>
typedef HINSTANCE DLL;
DLL * LOAD_DLL(char * name){
  DLL tmp = LoadLibraryA(name);
  if(!tmp){
    longjmp(LIBLOAD_FAILED, 1);
  }
  return &tmp;
}
#elif __APPLE__
#include <dlfcn.h>
typedef void* DLL;
DLL LOAD_DLL(char * name){
  char * path = (char *)malloc(strlen(name)+8);
  strcpy(path, "@rpath/");
  strcat(path, name);
  DLL tmp=dlopen(path, RTLD_LAZY);
  free(path);
  if (!handle){
    fprintf(stderr, "Loading dynamicly linked library failed: %s\n", dlerror());
    longjmp(LIBLOAD_FAILED, 1);
  }
  return tmp;
}
void * LOAD_FUNC(DLL lib, char * name){
  void * func=dlsym(lib, name);
  const char * emsg = dlerror();
  if(emsg){
    fprintf(stderr, "Loading function in dynamicly linked library failed: %s\n", emsg);
    longjmp(FUNCLOAD_FAILED, 1);
  }
  return func;
}
#elif __linux__
typedef void* DLL;
DLL LOAD_DLL(char * name){
  DLL tmp=dlopen(name, RTLD_LAZY);
  if (!handle){
    fprintf(stderr, "Loading dynamicly linked library failed: %s\n", dlerror());
    longjmp(LIBLOAD_FAILED, 1);
  }
  return tmp;
}
void * LOAD_FUNC(DLL lib, char * name){
  void * func=dlsym(lib, name);
  const char * emsg = dlerror();
  if(emsg){
    fprintf(stderr, "Loading function in dynamicly linked library failed: %s\n", emsg);
    longjmp(FUNCLOAD_FAILED, 1);
  }
  return func;
}
#elif __FREEDOS__
#error "DOCUMENTATION REQUIRED ON FREEDOS"
#else
#error "Unknown operating system"
#endif
#define UDLL_H 1
#endif
