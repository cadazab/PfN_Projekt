#ifndef PARSER_H
#define PARSER_H

#include "../protein.h"

Protein * parse(const char * filename);

/*
* Takes a Protein struct and frees itself and every content of it.
*/
void freeProteinStruct(Protein * protein);
char * readFromFile(const char *filename, unsigned long *filesize);
#endif
