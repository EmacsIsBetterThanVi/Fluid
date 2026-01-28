#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "pipe.h"
#include <setjmp.h>
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
bool stdinp = 0;
char * cOptions;
char * outputFile;
char * inputFileName;
FILE * inputFile;
char * mainFuncName;
jmp_buf buf;
static struct option clioptions[] = {
       {"verbose", no_argument, 0, 'v'},
       {"version", no_argument, 0, 'V'},
       {"no-cc", no_argument, 0, 'c'},
       {"input", required_argument, 0, 'I'},
       {"output", required_argument, 0, 'o'},
       {"c-options", required_argument, 0, 'C'},
       {"altmain", required_argument, 0, 'A'},
       {"help", no_argument, 0, 'h'},
       {"stdin", no_argument, 0, '@'}
       {0, 0, 0, 0}
};
const int major = 0;
const int minor = 1;
const int patch = 0;
int main(int argc, char ** argv){
    int c=0;
    int optindex = 0;
    while(c!=-1){
	c = getopt_long(argc, argv, ":vVcC:o:I:A:h@", clioptions, &optindex);
	switch (c){
	    case 'h':
	         printf("Fluid compiler version %d.%d.%d\nusage: fluid [options] file\n", major, minor, patch);
	         printf("-h or --help\t\t\t\tDisplays this help message\n");
		 printf("-V or --version\t\t\t\tDisplays the version number and exits\n");
		 printf("-v or --verbose\t\t\t\tDisplays extra debug info\n");
		 printf("-c or --no-cc\t\t\t\tDoes not trigger the C compiler\n");
		 printf("-C options or --c-options=options\tAdds extra options to the compilation stage\n");
		 printf("-I file or --input=file\t\t\tAn alternative way of specifying the input file\n");
		 printf("-o file or --output=file\t\tSets the name of the output file\n");
		 printf("-A prototype or --altmain=prototype\tSets the name of the main function\n");
	         return 0;
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
	         printf("Fluid version %d.%d.%d\n", major, minor, patch);
		 return 0;
	    case 'I':
	    	 inputp=1;
		 inputFileName = optarg;
		 break;
	    case 'A':
	    	 altmain = 1;
		 mainFuncName = optarg;
		 break;
	    case '@':
	         stdinp = 1;
		 break;
	}
    }
    if (optind < argc && !inputp){
      inputFileName = argv[argc-1];
    } else if(!inputp){
      	printf("no input file provided");
      	return 2;
    }
    if (stdinp) inputFile = ;
    else inputFile = fopen(inputFileName, "r");
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
    err=setjmp(buf);
    if(err!=0){
      return err;
    } else{
      compile(inputFile, outputFile, verbose_flag, mainFuncName);
      return 0;
    }
}
