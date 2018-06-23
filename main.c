#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "protein.h"

#define DIE() die_with_usage(argv[0]);


void die_with_usage(const char* progname) {
    fprintf(stderr, "USAGE: %s <f1>...<fn> [-n <n> -f -t] where:\n\n", progname);

    fprintf(stderr, "<fi> is a desired input-file and at least one file is required\n\n");

    fprintf(stderr, "-n is for the n-grams descriptor.\n");
    fprintf(stderr, "with an postive uneven number <n>\n\n");

    fprintf(stderr, "if -f is used, the input files will be interpreted\n"
                    "as files containing a list of filenames which\n"
                    "shall be analyzed\n\n");

    fprintf(stderr, "if -t is set, the programm will run predefined testcases\n"
    "and ignore all other parameters and the positional arguments\n"
    "<fi> are not required\n");

    exit(EXIT_FAILURE);
}

// TODO use function in parser.h/c as soon as it compiles and fixes its 
// termination problem in current implementation
char * readFromFile(const char *filename, unsigned long *filesize)
{
    FILE *fp = fopen(filename, "r");
    unsigned long fs;
    char *filecontent;
    
    if(fp == NULL)
    {
        fprintf(stderr, "%s: cannot open file \"%s\"\n", __func__, filename);
        return NULL;
    }
    if(fseek(fp, 0, SEEK_END) != 0)
    {
        fprintf(stderr, "%s: fseek failed\n", __func__);
    }
    fs = ftell(fp);
    rewind(fp);
    filecontent = malloc(sizeof(*filecontent) * (fs+1));
    if(fread(filecontent, sizeof(*filecontent), fs, fp) != fs)
    {
        fprintf(stderr, "%s: fread failed\n", __func__);
        return NULL;
    }
    filecontent[fs] = '\0';
    *filesize = fs+1;
    fclose(fp);

    return filecontent;
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

    const char* options = "n:tf";
    bool tflag, nflag, fflag;
    long ninput;
    unsigned int n, i;
    char **inputfiles, c;
    // we memorize the pointer to the memory containing a single files
    // content to free it later correctly
    unsigned long numoffiles, *startoffile;
    tflag = nflag = fflag = false;

    while((c = getopt(argc, argv, options)) != -1) {
        switch(c) {
            case 't': 
                tflag = true; 
                break;
            case 'n': 
                nflag = true;
                if(sscanf(optarg, "%ld", &ninput) != 1 || 
                ninput % 2 == 0 || ninput < 0) {
                    DIE();
                }
                n = (unsigned int) ninput;
                break;
            case 'f':
                fflag = true;
                break;
            case '?':
                DIE();
        }
    }
    numoffiles = (unsigned long) argc-optind;
    // maybe not secure
    inputfiles =  argv+optind;
    if(fflag) {
        // TODO think about how to improve code quality of this block
        unsigned long currentsize, maxnum = 1,
                      j, currindex = 0;
        char **content = malloc(maxnum * sizeof *content), *current;
        startoffile = malloc(numoffiles * sizeof *startoffile);
        assert(content != NULL);
        assert(startoffile != NULL);
        
        // iterate over all parameter files
        for(i = 0; i < numoffiles; i++) {
            current = readFromFile(inputfiles[i], &currentsize);
            // TODO think about how to avoid code duplication
            if(currindex == maxnum) {
                    maxnum *= 2;
                    content = realloc(content, maxnum * sizeof *content);
                    assert(content != NULL);
            }
            // this is the pointer to the memory malloced in read function
            content[currindex] = current;
            startoffile[i] = currindex;
            currindex++;


            for(j = 0; j < currentsize; j++) {
                printf("maxnum: %lu\n", maxnum);
                if(current[j] == '\n') {
                    if(currindex == maxnum) {
                        maxnum *= 2;
                        content = realloc(content, maxnum * sizeof *content);
                        assert(content != NULL);
                    }
                    printf("Index: %lu\n", currindex);
                    content[currindex] = current+j+1;
                    current[j] = '\0';
                    currindex++;
                }
            }
            printf("%s\n", current);
        }
        inputfiles = content;
        numoffiles = currindex;
    }

    if(!tflag && numoffiles == 0) {
        DIE();
    }
    if(tflag){
        printf("Test will be stated here\n");
    }
    else {
        if(nflag) {
            printf("N-Grams descriptor with %u will be started here\n", n);
        }
        else {
            printf("Angle descriptor will be started here\n");
        }
    call_function_for_files(inputfiles, numoffiles, print_func);
    }

    // we need to free the memory used for the input files in this case
    if(fflag) {
        for(i = 0; i < argc-optind; i++) {
            free(inputfiles[startoffile[i]]);
        }
        free(inputfiles);
        free(startoffile);
    }
    return EXIT_SUCCESS;
}