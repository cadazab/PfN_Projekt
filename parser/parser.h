#ifndef PARSER_H
#define PARSER_H

#include "../protein.h"

/* Takes a filename and returns a Protein struct. Please use a file with
* the conventions of the pdbe.
*/
Protein * parse(char * filename);

/*
* Takes a Protein struct and frees itself and every content of it.
*/
void freeProteinStruct(Protein * protein);
#endif
