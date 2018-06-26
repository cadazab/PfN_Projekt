#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "nGram.h"


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
double * calculateDistancesBetweenAllNGrams(nGram * ngrams,
                 unsigned long threshold)
{
    unsigned long nr_of_nGrams,i,j;
    
    if(ngrams == NULL)
    {
        fprintf(stderr,"nGram pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    nr_of_nGrams = sizeof(ngrams)/sizeof(ngrams[0]);
    double  arr[nr_of_nGrams][nr_of_nGrams];
    double *arrPointer = arr;
    
    for(i=0;i<nr_of_nGrams;i++)
    {
        for(j=0;j<nr_of_nGrams;j++)
        {
            arr[i][j] = calculateDistanceBetween2NGrams(ngrams[i],ngrams[j]);
            if(arr[i][j] > threshold)
            {
                arr[i][j] = (arr[i][j])*-1;
            }
        }
    }
    
    return arrPointer;
}
