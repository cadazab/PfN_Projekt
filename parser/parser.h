#ifndef PARSER_H
#define PARSER_H

#include "protein.h"

Protein * parse(char * filename);

void freeProteinStruct(Protein * protein);
#endif
