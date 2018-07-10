#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../ngrams/nGram.h"
#include "../protein.h"
#include "../descriptor.h"

//return the distance betweem 2 nGrams
double calculateDistanceBetween2NGrams(nGram * ngram1, nGram *ngram2)
{
    double distance;
    double x1, x2, y1, y2, z1, z2;
    
    if(ngram1 == NULL || ngram2 == NULL)
    {
        fprintf(stderr,"nGram pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    x1 = ngram1->central_cAlpha->x;
    x2 = ngram2->central_cAlpha->x;
    y1 = ngram1->central_cAlpha->y;
    y2 = ngram2->central_cAlpha->y;
    z1 = ngram1->central_cAlpha->z;
    z2 = ngram2->central_cAlpha->z;
    
    distance = sqrt(pow(x1-x2,2)+pow(y1-y2,2)+pow(z1-z2,2));
    
    return distance;
}


//Return a 2D array containing the distances between all nGrams,
//where positive nunmbers are the distances that are below the threshold,
//and negative numbers are the distances above the threshold.
//This enables the adjusting of the threshold without having to
//recalculate all distances.
double **calculateDistancesBetweenAllNGrams(nGram * ngrams,
            unsigned long nofngrams, double threshold) {
    unsigned long i,j;
    
    if(ngrams == NULL)
    {
        fprintf(stderr,"nGram pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    double **arr = malloc(nofngrams * sizeof *arr);
    assert(arr != NULL);
    for(i = 0; i < nofngrams; i++)
    {
        arr[i] = malloc(nofngrams * sizeof *arr[i]);
        assert(arr[i] != NULL);
        for(j = 0; j < nofngrams; j++) {
            arr[i][j] = calculateDistanceBetween2NGrams(ngrams+i,ngrams+j);
            if(arr[i][j] > threshold) {
                arr[i][j] = -arr[i][j];
            }
        }
    }
    
    return arr;
}

/*
Free Space allocated while calculation
*/
void free_distances(double **distances, unsigned long nofngrams) {
    unsigned long i;
    if(distances != NULL) {
        for(i = 0; i < nofngrams; i++) {
            free(distances[i]);
        }
        free(distances);
    }
}

void distance_descriptor(const Protein *p, const char *outfile,
                        const void *data) {
    unsigned long n = *((unsigned long *) data);
    unsigned long nofngrams = numberOfNGrams(p, n), i, j;
    nGram *ngrams = generateNGrams(p, n);
    double **distances = calculateDistancesBetweenAllNGrams(ngrams, nofngrams, 10.0);
    FILE *out = fopen(outfile, "w");
    assert(outfile != NULL);
    fprintf(out, "distances");
    for(i = 0; i < nofngrams; i++)
    {
        fprintf(out, ", %lu", i);
    }

    for(i = 0; i < nofngrams; i++) {
        fprintf(out, "\n%lu", i);
        for(j = 0; j < nofngrams; j++) {
            fprintf(out, ", %lf", distances[i][j]);
        }
    }
    fclose(out);

    free_distances(distances, nofngrams);
}