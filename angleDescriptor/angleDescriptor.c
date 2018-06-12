#include <stdlib.h>

struct Vektor{
  int x;
  int y;
  int z;
}

Vektor Kreuzprodukt(Vektor a, Vektor b){
  Vektor ergebnis = new Vektor;
  ergebnis.x = a.y*b.z - a.z*b.y;
  ergebnis.y = a.z*b.x - a.x*b.z;
  ergebnis.z = a.x*b.y - a.y*b.x;
  return ergebnis;
}
  
