#ifndef NGRAM_H
#define NGRAM_H

typedef struct {
    Atom **cAlphas;
    Atom *central_cAlpha;
    unsigned long nr_cAlphas;
} nGram;

unsigned long numberOfNGrams(const Protein * protein, unsigned long n);
nGram * generateNGrams(const Protein * protein, unsigned long n);
void deleteNGrams(nGram * ngrams);
#endif
