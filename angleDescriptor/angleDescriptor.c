#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../protein.h"
#include "../nGram.h"

typedef struct Vector{
  double x;
  double y;
  double z;
}Vector;

typedef struct Angle{
  double angle;
  double distance;
}Angle;

Vector cross_product(Vector a, Vector b){
  Vector result;
  result.x = a.y*b.z - a.z*b.y;
  result.y = a.z*b.x - a.x*b.z;
  result.z = a.x*b.y - a.y*b.x;
  return result;
}

double scalar_product(Vector a, Vector b){
  double result;
  result = a.x*b.x + a.y*b.y + a.z*b.z;
  return result;
}

double abs_val(Vector a){
  double result;
  result = sqrt(pow(a.x,2) + pow(a.y,2) + pow(a.z,2));
  return result;
}

Vector normalization(Vector a){
  double abs = abs_val(a);
  Vector result;
  result.x = a.x/abs;
  result.y = a.y/abs;
  result.z = a.z/abs;
  return result;
}

Vector difference(Vector a, Vector b){
  Vector result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return result;
}

double angle(Vector a, Vector b, Vector c, Vector d){
  Vector v1 = difference(a,b);
  Vector v2 = difference(b,c);
  Vector v3 = difference(c,d);
  return acos(scalar_product(normalization(cross_product(v1,v2)),normalization(cross_product(v2,v3)))) * 180 / M_PI;
}

Vector create_vector(Atom atom){
  Vector result;
  result.x = atom.x;
  result.y = atom.y;
  result.z = atom.z;
  return result;
}

Angle get_angle(Protein protein){
  Angle* result;
  unsigned long nr_ngrams = numberOfNGrams(&protein,6);
  result = malloc(sizeof(*result) * nr_ngrams);
  nGram* n_grams = generateNGrams(protein,6);
  unsigned int i;
  for(i = 0; i < nr_ngrams; i++){
    Vector v1,v2,v3,v6;
    v1 = create_vector(n_grams[i].cAlphas[0]);
    v2 = create_vector(n_grams[i].cAlphas[1]);
    v3 = create_vector(n_grams[i].cAlphas[2]);
    v6 = create_vector(n_grams[i].cAlphas[5]);
    result[i].angle = angle(v1,v2,v3,v6);
    result[i].distance = abs_val(difference(v1,v6));
  }
  return result;
}

void free_angle(*Angle angle){
  free(angle);
}

int main(int argc, char *argv[]){
  Vector a;
  Vector b;
  Vector c;
  Vector d;
  a.x=1;
  a.y=0;
  a.z=1;
  b.x=0;
  b.y=0;
  b.z=0;
  c.x=0;
  c.y=1;
  c.z=0;
  d.x=0;
  d.y=1;
  d.z=1;
  printf("angle:%f°\n",angle(a,b,c,d));
  return EXIT_SUCCESS;
}

  
