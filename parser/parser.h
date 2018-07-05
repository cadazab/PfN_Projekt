#ifndef PARSER_H
#define PARSER_H

#include "../protein.h"

Protein * parse(const char * filename);

void freeProteinStruct(Protein * protein);
char * readFromFile(const char *filename, unsigned long *filesize);
#endif
