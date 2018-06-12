#include <stdlib.h>
#include <math.h>

struct Vektor{
  double x;
  double y;
  double z;
}

Vektor Kreuzprodukt(Vektor a, Vektor b){
  Vektor ergebnis = new Vektor;
  ergebnis.x = a.y*b.z - a.z*b.y;
  ergebnis.y = a.z*b.x - a.x*b.z;
  ergebnis.z = a.x*b.y - a.y*b.x;
  return ergebnis;
}

double Skalarprodukt(Vektor a, Vektor b){
  double ergebnis;
  ergebnis = a.x*b.x + a.y*b.y + a.z*b.z;
  return ergebnis;
}

double Betrag(Vektor a){
  double ergebnis;
  ergebnis = sqrt(pow(a.x,2) + pow(a.y,2) + pow(a.z,2));
  return ergebnis;
}

Vektor Normierung(Vektor a){
  double betrag = Betrag(a);
  Vektor ergebnis = new Vektor;
  ergebnis.x = a.x/betrag;
  ergebnis.y = a.y/betrag;
  ergebnis.z = a.z/betrag;
  return ergebnis;
}


  
