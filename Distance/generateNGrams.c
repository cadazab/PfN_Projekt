#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "protein.h"
#include "nGram.h"



Protein * generateTestProtein(unsigned long cAlphas)
{
    Protein * protein;
    Atom * atom;
    size_t i;
    
    atom = malloc(cAlphas*sizeof(Atom));
    protein = malloc(sizeof(Protein));
    protein->cAlphas = malloc(cAlphas*sizeof(Atom));
    protein->nr_residues = cAlphas;
    for(i=0;i<cAlphas;i++)
    {
        if(i==cAlphas/2)
        {
            atom[i].name = "centerAtom";
        }
        else
        {
            atom[i].name = "Atom";
        }
        protein->cAlphas[i] = atom[i];
    }
    
    return protein;
}


void deleteTestProtein(Protein * protein)
{
    size_t i;
    
    for(i=0;i<protein->nr_residues;i++)
    {
        free(&protein->cAlphas[i]);
    }
    free(protein->cAlphas);
    free(protein);
}


nGram * generateNGrams(Protein * protein, unsigned long alphas)
{
    size_t i;
    nGram * ngram;
    
    assert(alphas <= protein->nr_residues);
    
    ngram = malloc(sizeof(nGram));
    ngram->nr_cAlphas = alphas;
    ngram->cAlphas = malloc(alphas*sizeof(Atom));
    for(i=0;i<alphas;i++)
    {
        ngram->cAlphas[i] = protein->cAlphas[i];
    }
    ngram->central_cAlpha = &ngram->cAlphas[alphas/2];
    
    return ngram;
}


void deleteNGrams(nGram * ngram)
{
    free(ngram->cAlphas);
    free(ngram);
}


void printNGram(nGram * ngram)
{
    size_t i;
    
    printf("number of c-Alphas: %lu\n",ngram->nr_cAlphas);
    printf("Atoms: ");
    for(i=0;i<ngram->nr_cAlphas;i++)
    {
        printf("%s ",ngram->cAlphas[i].name);
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
    Protein * protein;
    nGram * ngram;
    unsigned long calphas;
    
    if(sscanf(argv[1],"%lu",&calphas) != 1)
    {
        fprintf(stderr,"sscanf failed\n");
        return EXIT_FAILURE;
    }
    printf("input: %lu\n",calphas);
    protein = generateTestProtein(10);
    ngram = generateNGrams(protein, calphas);
    printNGram(ngram);
    
    return EXIT_SUCCESS;
}
