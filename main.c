#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "protein.h"
#include "descriptor.h"
#include "parser/parser.h"
#include "ngrams/nGram.h"
#include "angleDescriptor/angleDescriptor.h"

#define DIE() die_with_usage(argv[0]);

typedef Protein* Parsefunc (const char *filename);

/*
Generates the filename for the output file by adding a file extension
*/
const char *generate_outfile_name(const char *filename) {
    const char *ending = "_result.csv";
    unsigned long length = strlen(filename)+strlen(ending)+1;
    char *outputfile = malloc((length)*sizeof(char));
    assert(outputfile != NULL);
    snprintf(outputfile, length, "%s%s", filename, ending);
    return outputfile;
}
/*
Reads in a protein by using parser and creates output by calling descriptor
for each file in files
*/
void analyze_all_proteins(Descriptor *descriptor, char * const *files,
                        unsigned long numoffiles, Parsefunc parser) {
    unsigned long i;
    Protein *p;
    for (i = 0; i < numoffiles; i++) {
        const char *outfile = generate_outfile_name(files[i]);
        p = parser(files[i]);
        descriptor(p , outfile);
        free((void*) outfile);
        freeProteinStruct(p);
    }

}

/*
print a usage and exit the programm
*/
static void die_with_usage(const char* progname) {
    fprintf(stderr, "USAGE: %s <f1>...<fn> [-n <n> -f -t] where:\n\n", progname);

    fprintf(stderr, "<fi> is a desired input-file and at least one file is required\n\n");

    fprintf(stderr, "-n is for the n-grams descriptor.\n");
    fprintf(stderr, "with an postive uneven number <n>\n\n");

    fprintf(stderr, "if -f is used, the input files will be interpreted\n"
                    "as files containing a filename of a file which\n"
                    "shall be analyzed in each line\n\n");

    fprintf(stderr, "if -t is set, the programm will run predefined testcases, \n"
    "ignore all other parameters and the positional arguments\n"
    "<fi> are not required\n");

    exit(EXIT_FAILURE);
}

/*
adding to an char* array with exponential 
growth in size and adjust the variables
*/
char **add_to_array_exp(char **array, unsigned long *currentsize, 
                        unsigned long *maxsize, char* elem) {
    assert(array != NULL);
    assert(currentsize != NULL);
    assert(maxsize != NULL);
    assert((*currentsize) <= (*maxsize));
    
    if(*currentsize == *maxsize) {
        *maxsize *= 2;
        array = realloc(array, *maxsize * sizeof *array);
    }
    array[(*currentsize)] = elem;
    (*currentsize)++;

    return array;
}

/*
interpret the strings in inputfiles as files contaning filenames
and return an array of filenames
*/
char **parse_files(char **inputfiles, unsigned long *filestarts, 
                    unsigned long *numoffiles) {
    unsigned long currentsize, maxnum = 10, j, currindex = 0, i;
    char **content = malloc(maxnum * sizeof *content), *current;
    assert(content != NULL);
    assert(filestarts != NULL);
    
    // iterate over all parameter files
    for(i = 0; i < *numoffiles; i++) {
        current = readFromFile(inputfiles[i], &currentsize);
        assert(current != NULL);
        // at content[currindex] is the pointer malloced for the ith file
        filestarts[i] = currindex;
        content = add_to_array_exp(content, &currindex, &maxnum, current);
        for(j = 0; j < currentsize; j++) {
            if(current[j] == '\n') {
                current[j] = '\0';
                content = add_to_array_exp(content, &currindex, 
                                            &maxnum, current+j+1);
            }
        }
    }
    *numoffiles = currindex;

    return content;
}

/*
this function parses the options from argc to flags and n
argv gets permuted by getopt
*/
void parse_options(int argc, char **argv, bool *flags, unsigned int *n) {
    const char* options = "tn:f";
    char c;
    long ninput;
    while((c = getopt(argc, argv, options)) != -1) {
        switch(c) {
            case 't':
                flags[0] = true;
                break;
            case 'n':
                flags[1] = true;
                if(sscanf(optarg, "%ld", &ninput) != 1 || 
                ninput % 2 == 0 || ninput < 0) {
                    DIE();
                }
                *n = (unsigned int) ninput;
                break;
            case 'f':
                flags[2] = true;
                break;
            case '?':
                DIE();
        }
    }
}


int main(int argc, char *argv[]){

    bool tflag, nflag, fflag;
    // this for convenience in optionflag functions
    // order is the same as in string options in the function
    bool flags[] = {false, false, false};

    unsigned int n, i;
    char **inputfiles;
    // we save the pointer to the memory containing a
    // single files content to free it later correctly
    unsigned long numoffiles, *filestarts;

    // Initilize function pointer
    Parsefunc *parser = &parse;
    Descriptor *descriptor = NULL;


    parse_options(argc, argv, flags, &n);

    tflag = flags[0];
    nflag = flags[1];
    fflag = flags[2];

    numoffiles = (unsigned long) argc-optind;
    // maybe not secure
    inputfiles =  argv+optind;

    // no inputfiles and not testing
    if(!tflag && numoffiles == 0) {
        DIE();
    }

    if(fflag) {
        filestarts = malloc(numoffiles * sizeof *filestarts);
        inputfiles = parse_files(inputfiles, filestarts, &numoffiles);
    }

    if(tflag){
        printf("Test will be stated here\n");
        return EXIT_SUCCESS;
    }
    else {

        // TODO call real angle descriptor
        if(nflag) {
            printf("N-Grams descriptor with %u will be started here\n", n);
        }
        else {
            printf("Angle descriptor will be started here\n");
            descriptor = &angle_descriptor;

        }
        analyze_all_proteins(descriptor, inputfiles, numoffiles, parser);

        // we need to free the memory used for the input files in this case
        if(fflag) {
            for(i = 0; i < argc-optind; i++) {
                free(inputfiles[filestarts[i]]);
            }
            free(inputfiles);
            free(filestarts);
        }
    }

    return EXIT_SUCCESS;
}
