// Header file for the parser for GML files
//
// Mark Newman  11 AUG 06

#ifndef _READGML_H
#define _READGML_H

#include <stdio.h>
#include "struct.h"

int read_prof_escola(PROF_ESC *G, FILE *stream);
void free_memo(PROF_ESC *G);

#endif
