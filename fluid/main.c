#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "pipe.h"
#ifndef _WIN32
#include <unistd.h>
#include <getopt.h>
#endif 
char * code;
char * output;
extern int compile(FILE *, char *, bool, char *);
bool verbose_flag = 0;
bool altmain = 0;
bool coptions = 0;
bool outputp = 0;
bool inputp = 0;
bool no_cc = 0;
char * cOptions;
char * outputFile;
FILE * inputFile;
char * mainFuncName;
static struct option clioptions[] = {
       {"verbose", no_argument, 0, 'v'},
       {"version", no_argument, 0, 'V'},
       {"no-cc", no_argument, 0, 'c'},
       {"input", required_argument, 0, 'I'},
       {"output", required_argument, 0, 'o'},
       {"c-options", required_argument, 0, 'C'},
       {"altmain", required_argument, 0, 'A'},
       {0, 0, 0, 0}
};
int main(int argc, char ** argv){
    int c=0;
    int optindex = 0;
    while(c!=-1){
	c = getopt_long(argc, argv, ":vVcC:o:I:A:", clioptions, &optindex);
	switch (c){
	    case 'C':
	    	 coptions=1;
		 no_cc = 0;
		 cOptions = optarg;
		 break;
	    case 'o':
	    	 outputp=1;
		 outputFile = optarg;
		 break;
	    case 'v':
	    	 verbose_flag = 1;
		 break;
	    case 'c':
	    	 no_cc = 1;
		 break;
	    case 'V':
	    	 printf("Fluid version 0.0.0");
		 return 0;
	    case 'I':
	    	 inputp=1;
		 inputFile = fopen(optarg, "r");
		 break;
	    case 'A':
	    	 altmain = 1;
		 mainFuncName = optarg;
		 break;
	}
    }
    if (optind < argc && !inputp){
       inputFile=fopen(argv[argc-1], "r");
    } else if(!inputp){
      	printf("no input file provided");
      	return 2;
    }
    if(!outputp){
	if(no_cc){
		outputFile = "fluid.out.c";
	} else {
	       outputFile = "a.out";
	}
    }
    if(!altmain){
	mainFuncName = "void main(String[])"; //Function prototype does not care about argument names. A full function name such as {CLASS}${FUNCTION}${OVERLOAD} is also valid
    }
    int err=0;
    if(err=setjmp()){
      return err;
    } else{
      compile(inputFile, outputFile, verbose_flag, mainFuncName);
      return 0;
    }
}
