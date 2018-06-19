#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>

#include "protein.h"

#define DIE() die_with_usage(argv[0]);


void die_with_usage(const char* progname) {
    fprintf(stderr, "USAGE: %s <f1>...<fn> [-n <n> -t] where:\n\n", progname);

    fprintf(stderr, "<fi> is a desired input-file\n\n");

    fprintf(stderr, "-n is for the n-grams descriptor.\n");
    fprintf(stderr, "with an uneven number <n>\n\n");

    fprintf(stderr, "if -t is set, the programm will run predefined testcases"
    " and ignore all other parameters\n");

    exit(EXIT_FAILURE);
}

void print_func(char* file) {
    printf("Filename: %s\n", file);
}

void call_function_for_files(char **files, unsigned int numoffiles, void (*f)(char*)) {
    unsigned int i; 
    for(i = 0; i < numoffiles; i++) {
        f(files[i]);
    }
}

int main(int argc, char *argv[]){

    const char* options = "n:t";
    bool tflag, nflag;
    long input;
    unsigned long n;
    char **inputfiles;
    unsigned int numoffiles;
    tflag = nflag = false;
    char c;

    while((c = getopt(argc, argv, options)) != -1) {
        switch(c) {
            case 't': tflag = true; break;
            case 'n': 
                nflag = true;
                if(sscanf(optarg, "%ld", &input) != 1 || 
                input % 2 == 0 || input < 0) {
                    DIE();
                }
                n = (unsigned long) input;
                break;
            case '?': DIE();
        }
    }

    numoffiles = argc-optind;
    inputfiles = argv+optind;
    if(numoffiles == 0) {
        DIE();
    }
    if(tflag){
        printf("Test will be stated here\n");
    }
    else {
        if(nflag) {
            printf("N-Grams descriptor with %lu will be started here\n", n);
        }
        else {
            printf("Angle descriptor will be started here\n");
        }
    call_function_for_files(inputfiles, numoffiles, print_func);
    }
    return EXIT_SUCCESS;
}