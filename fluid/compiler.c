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
extern jmp_buf buf;
extern int main(int argc, char ** argv);
char * Cfile;
char * tokens[256];
int CFilePtr = 0;
enum Ntype{
  ClassT,
  NamespaceT,
  EnumT,
  FunctionT,
  VariableT,
  ConstantT
};
bool IsSyntaticChar(char nextchar){
  return (nextchar != '=' && nextchar != '<' && nextchar != '>' && nextchar != ' ' && nextchar != '\n' && nextchar != '{' && nextchar != '\t' && nextchar != '}' && nextchar != '[' && nextchar != ']' && nextchar != '(' && nextchar != ')' && nextchar != '!' && nextchar != '&' && nextchar != '*' && nextchar != ',' && nextchar != '+' && nextchar != '-' && nextchar != '/' && nextchar != '|' && nextchar != '^');
}
bool IsCollector(char nextchar){
  return (nextchar != '<' && nextchar != '>' &&  nextchar != '{' && nextchar != '}' && nextchar != '[' && nextchar != ']' && nextchar != '(' && nextchar != ')');
}
bool IsOperator(char nextchar){
  return (nextchar != '=' && nextchar != '<' && nextchar != '>' && nextchar != '[' && nextchar != ']' &&  nextchar != '!' && nextchar != '&' && nextchar != '*'  && nextchar != '+' && nextchar != '-' && nextchar != '/' && nextchar != '|' && nextchar != '^');
}
typedef struct name {
  char name[256];
  enum Ntype type;
  char TypeName[256];
} Name;
void ClearTokens(){
  for(int i=0; i<256; i++){
    free(tokens[i]);
    tokens[i] = NULL;
  }
}
typedef struct Layer {
  int Line;
  char Type;
} Layer;
void ConstructC(){
}
void TokenRead(int * start, int end, char * string, char * CODE){
  int Tptr = 0;
  for(int i=start[0]; i<end; i++){
    if(CODE[i]!=' ' && CODE[i]!='\n' && CODE[i]!='\t' && CODE[i]!='\r'){
      string[Tptr] = CODE[i];
      Tptr++;
    }
  }
  start[0]=end+1;
}
Name * names[256];
bool HasMainFunc = false;
int layer=0;
Layer layers[64];
char package[256];
char class[256];
#define READ TokenRead(&last, i, tokens[TokenCounter], ProgramContent)
int compile(FILE * inf, char * outf, bool verbose, char * mainFunc){
  #if UINTPTR_MAX == 0xFFFFFFF
  Cfile = malloc(16777216);
  #elif UINTPTR_MAX == 0xFFFF
  Cfile = malloc(65536);
  #else
  Cfile = malloc(1073741824);
  #endif
  fseek(inf, 0, SEEK_END);
  int length = ftell(inf)+3;
  char * ProgramContent = malloc(length);
  fseek(inf, 0, SEEK_SET);
  fread(ProgramContent, 1, length, inf);
  int last=0;
  int TokenCounter = 0;
  int Line = 1;
  for(int i=0; i<length; i++){
    if(ProgramContent[i] == 0){ // Run Clean up code, and exit
      if (layer == 0) return 0;
      else {
	printf("%d layers still on the stack:\n", layer);
	for (int i=layer-1; i >= 0; i--){
	  printf("Unterminated %c on line %d\n", layers[i].Type, layers[i].Line);
	}
	return 1;
      }
    } else if(ProgramContent[i] == '}' && layer == 1){
       if (!HasMainFunc){
	char * fullPath = malloc(1024);
	strcpy(fullPath, getenv("HOME"));
	strcat(fullPath, "/.fluid/");
	strcat(fullPath, package);
	strcat(fullPath, ".");
	strcat(fullPath, class);
	FILE * plist = fopen(fullPath, "wb");
       }
       layer = 0;
    } else if (ProgramContent[i] == '{'){
      layers[layer].Line = Line;
      layers[layer].Type = '{';
      layer++;
    } else if (ProgramContent[i] == '('){
      layers[layer].Line = Line;
      layers[layer].Type = '(';
      layer++;
    }else if (ProgramContent[i] == '['){
      layers[layer].Line = Line;
      layers[layer].Type = '[';
      layer++;
    } else if (ProgramContent[i] == '}'){
      if (layers[layer-1].Type == '{')layer--;
      else {
	printf("Unexpected } on line %d\n", Line);
	return 2;
      }
    } else if (ProgramContent[i] == ')'){
      if (layers[layer-1].Type == '(')layer--;
      else {
	printf("Unexpected ) on line %d\n", Line);
	return 2;
      }
    }else if (ProgramContent[i] == ']'){
      if (layers[layer-1].Type == '[')layer--;
      else {
	printf("Unexpected ] on line %d\n", Line);
	return 2;
      }
    } else if (ProgramContent[i] == '\n'){
      Line++;
    }else if(ProgramContent[i] == ';'){
      ConstructC();
      ClearTokens();
      last=i+1;
    } else if (ProgramContent[i] == ' '){
      
    } /* else if(ProgramContent[i]==' '){
     if (IsSyntaticSpace(ProgramContent[i+1])){
	tokens[TokenCounter] = malloc(i-last);
	tokenType[TokenCounter]=KEY;
	READ;
      } else if (ProgramContent[i+1] != '=' && TokenCounter == 0){
        tokens[TokenCounter] = malloc(i-last);
	tokenType[TokenCounter]=KEY;
	READ;
      }
    } else if(ProgramContent[i] == '=' && ProgramContent[i+1] != '='){
        tokens[TokenCounter] = malloc(i-last);
	tokenType[TokenCounter]=VAR;
	READ;
    }*/
  }
  return 0;
}
