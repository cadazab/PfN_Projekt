#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "protein.h"
#include "nGram.h"


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
