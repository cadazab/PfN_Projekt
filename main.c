#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>

#include "protein.h"

#define DIE() die_with_usage(argv[0]);

void die_with_usage(const char* progname) {
    fprintf(stderr, "USAGE: %s <f> [-n <n> -t] where:\n\n", progname);

    fprintf(stderr, "<f> is the desired output-file\n\n");

    fprintf(stderr, "-n is for the n-grams descriptor.\n");
    fprintf(stderr, "with an uneven number <n>\n\n");

    fprintf(stderr, "if -t is set, the programm will run predefined testcases"
    " and ignore all other parameters\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){

    const char* options = "n:t";
    bool tflag, nflag;
    long input;
    unsigned long n;
    char* outputfile;
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
    if(argc - optind != 1) {
        fprintf(stderr, "Invalid number of positional parameters\n");
        DIE();
    }
    outputfile = argv[optind];

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
        printf("Results will be written as CSV to %s\n", outputfile);
    }
    return EXIT_SUCCESS;
}