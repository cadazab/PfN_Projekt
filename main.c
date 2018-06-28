#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "protein.h"
#include "parser/parser.h"

#define DIE() die_with_usage(argv[0]);

typedef Protein* Parsefunc (const char *filename);
typedef void Fileprocessorfunc (const char *filename, void *data);
typedef void* Desciptorfunc (Protein* protein);

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
TODO remove this debug function
*/
void print_func(const char *file , void *data) {
    (void) data;
    printf("Filename: %s\n", file);
}

/*
call f for each file in files
*/
static void call_processfunc_for_all(char **files, unsigned int numoffiles,
                                Fileprocessorfunc f, void **data) {
    assert(files != NULL);
    unsigned int i; 
    for(i = 0; i < numoffiles; i++) {

        if(data != NULL) f(files[i], data[i]);
        else f(files[i], NULL);
    }
}

static Protein **call_parsefunc_for_all(char **files,
                                            unsigned long numoffiles,
                                            Parsefunc p) {
    Protein **results = malloc(numoffiles * sizeof *results);
    unsigned long i;
    for(i = 0; i < numoffiles; i++) {
        results[i] = p(files[i]);
    }
    return results;
}

static void **call_descriptorfunc_for_all(Protein **proteins,
                                             unsigned long number,
                                             Desciptorfunc d) {
    void **results = malloc(number * sizeof *results);
    unsigned long i;
    for(i = 0; i < number; i++) {
        results[i] = d(proteins[i]);
    }
    return results;
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
this function parses the options from argc to flags

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
TODO write actual data to a file
*/
void write_output_file(const char *filename, void *data) {
    (void) data;
    const char *outputfile = generate_outfile_name(filename);

    printf("Write output for %s to %s\n", filename, outputfile);
    free((void*)outputfile);
}

void *dummy_descriptor(Protein *protein) {
    if(protein == NULL) {
        printf("Processing: NULL\n");
    }
    else {
        printf("Processing: %s\n", protein->name);
    }
    return NULL;
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
    Protein **proteins;

    // Declare function pointer
    Parsefunc *parser = &parse;
    Desciptorfunc *desc = &dummy_descriptor;
    void **descresult;


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

        proteins = call_parsefunc_for_all(inputfiles, numoffiles, parser);

        // TODO call real descriptor functions
        if(nflag) {
            printf("N-Grams descriptor with %u will be started here\n", n);
        }
        else {
            printf("Angle descriptor will be started here\n");
        }
        descresult = call_descriptorfunc_for_all(proteins, numoffiles, desc);
        call_processfunc_for_all(inputfiles, numoffiles, print_func,
                                    descresult);

        call_processfunc_for_all(inputfiles, numoffiles, write_output_file, NULL);

        // we need to free the memory used for the input files in this case
        if(fflag) {
            for(i = 0; i < argc-optind; i++) {
                free(inputfiles[filestarts[i]]);
            }
            free(inputfiles);
            free(filestarts);
        }
        free(proteins);
        free(descresult);
    }

    return EXIT_SUCCESS;
}
