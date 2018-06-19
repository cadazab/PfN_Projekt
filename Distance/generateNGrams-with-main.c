#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "protein.h"
#include "nGram.h"


//generate a protein for testing purposes
Protein * generateTestProtein(unsigned long cAlphas)
{
    Protein * protein;
    Atom * atom;
    size_t i;
    
    assert(cAlphas > 0);
    
    atom = malloc(cAlphas*sizeof(Atom));
    protein = malloc(sizeof(Protein));
    protein->cAlphas = malloc(cAlphas*sizeof(Atom));
    protein->nr_residues = cAlphas;
    
    atom[0].name = "firstAtom";
    protein->cAlphas[0] = atom[0];
    for(i=1;i<cAlphas;i++)
    {
        if(i == cAlphas-1)
        {
            atom[i].name = "lastAtom";
        }
        else
        {
            atom[i].name = "Atom";
        }
        protein->cAlphas[i] = atom[i];
    }
    
    return protein;
}


//delete the testing protein
void deleteTestProtein(Protein * protein)
{
    size_t i;
    
    assert(protein != NULL);
    
    for(i=0;i<protein->nr_residues;i++)
    {
        free(&protein->cAlphas[i]);
    }
    free(protein->cAlphas);
    free(protein);
}


//generate nGrams from a given protein
nGram * generateNGrams(Protein * protein, unsigned long n)
{
    size_t i;
    size_t j;
    nGram * ngrams;
    unsigned long number_of_ngrams;
    
    assert(n <= protein->nr_residues);
    
    number_of_ngrams = protein->nr_residues - n + 1;
    ngrams = malloc(number_of_ngrams*sizeof(nGram));
    for(i=0;i<number_of_ngrams;i++)
    {
        ngrams[i].nr_cAlphas = n;
        ngrams[i].cAlphas = malloc(n*sizeof(Atom));
        for(j=0;j<n;j++)
        {
            ngrams[i].cAlphas[j] = protein->cAlphas[j+i];
        }
        ngrams[i].central_cAlpha = &ngrams[i].cAlphas[n/2];
    }
    return ngrams;
}


//delete nGrams
void deleteNGrams(nGram * ngrams)
{
    assert(ngrams != NULL);
    
    free(ngrams->cAlphas);
    free(ngrams);
}


//print c-Alphas for a given protein for testing purposes
void printfProtein(Protein * protein)
{
    size_t i;
    
    assert(protein != NULL);
    
    printf("Protein(%lu): ",protein->nr_residues);
    for(i=0;i<protein->nr_residues;i++)
    {
        printf("%s ",protein->cAlphas[i].name);
    }
    printf("\n");
}


//print c-Alphas for a given nGram for testing purposes
void printNGrams(Protein * protein, nGram * ngrams)
{
    size_t i;
    size_t j;
    
    assert(protein != NULL);
    assert(ngrams != NULL);
    
    for(j=0;j<protein->nr_residues - ngrams[0].nr_cAlphas + 1;j++)
    {
        printf("nGram(%lu): ",ngrams[j].nr_cAlphas);
        for(i=0;i<ngrams[j].nr_cAlphas;i++)
        {
            printf("%s ",ngrams[j].cAlphas[i].name);
        }
        printf("\n");
    }
}


//main method for testing purposes
int main(int argc, char *argv[])
{
    Protein * protein;
    nGram * ngram;
    unsigned long proteinLength;
    unsigned long calphas;
    
    if(sscanf(argv[1],"%lu",&proteinLength) != 1 
        || sscanf(argv[2],"%lu",&calphas) != 1)
    {
        fprintf(stderr,"sscanf failed\n");
        return EXIT_FAILURE;
    }
    protein = generateTestProtein(proteinLength);
    ngram = generateNGrams(protein, calphas);
    printfProtein(protein);
    printNGrams(protein, ngram);
    
    return EXIT_SUCCESS;
}
