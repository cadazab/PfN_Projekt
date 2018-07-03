#ifndef PROTEIN_H
#define PROTEIN_H

typedef struct {
    char *name;
    double x, y, z;
}  Atom;

typedef struct {
    char *name;
    Atom *atoms;
    unsigned long nr_atoms;
} Residue;

typedef struct {
    char *name;
    Residue **residues; 
    Atom **atoms;
    Atom **cAlphas;
    unsigned long nr_residues;
    unsigned long nr_atoms;
} Protein;

#endif
