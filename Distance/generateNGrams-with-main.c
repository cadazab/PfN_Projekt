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


//return the number of nGrams a Protein would consist of for a given n
unsigned long numberOfNGrams(const Protein * protein, unsigned long n)
{
    if(protein == NULL)
    {
        fprintf(stderr,"Protein pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    if(n > protein->nr_residues)
    {
        fprintf(stderr,
        "%lu is smaller than the number of c-Alphas in the protein\n",n);
        exit(EXIT_FAILURE);
    }
    
    return protein->nr_residues - n + 1;
}


//generate nGrams from a given protein
nGram * generateNGrams(const Protein * protein, unsigned long n)
{
    size_t i;
    size_t j;
    nGram * ngrams;
    unsigned long number_of_ngrams;
    
    if(protein == NULL)
    {
        fprintf(stderr,"Protein pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    if(n > protein->nr_residues)
    {
        fprintf(stderr,
        "%lu is smaller than the number of c-Alphas in the protein\n",n);
        exit(EXIT_FAILURE);
    }
    
    number_of_ngrams = numberOfNGrams(protein, n);
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
    if(ngrams == NULL)
    {
        fprintf(stderr,"nGram pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    free(ngrams->cAlphas);
    free(ngrams);
}


//print c-Alphas for a given protein for testing purposes
void printfProtein(const Protein * protein)
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
void printNGrams(const Protein * protein, nGram * ngrams)
{
    size_t i;
    size_t j;
    unsigned long number_of_ngrams;
    
    assert(protein != NULL);
    assert(ngrams != NULL);
    
    
    number_of_ngrams = numberOfNGrams(protein, ngrams[0].nr_cAlphas);
    for(j=0;j<number_of_ngrams;j++)
    {
        printf("nGram(%lu): ",j);
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
    const Protein * protein;
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
    printf("number of nGrams: %lu\n",numberOfNGrams(protein, calphas));
    
    return EXIT_SUCCESS;
}
