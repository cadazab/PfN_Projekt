#include "protein.h"

struct Protein {
    char *name;
    Residue *residues; 
    Atom *atoms;
    unsigned long nr_residues;
    unsigned long nr_atoms;
};

struct Residue {
    char *name;
    Atom *atoms;
    unsigned long nr_atoms;
};  

struct Atom {
    char *name;
    double *coordinates;
};  

