#include <stdlib.h>
#include <stdio.h>
#include "../protein.h"
#include "nGram.h"


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
        ngrams[i].central_cAlpha = ngrams[i].cAlphas[n/2];
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
