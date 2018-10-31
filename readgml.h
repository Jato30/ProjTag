// Header file for the parser for GML files
//
// Mark Newman  11 AUG 06

#ifndef _READGML_H
#define _READGML_H

#include <stdio.h>
#include "struct.h"
#include "network.h"

int read_prof_escola(PROF *professor, ESCOLA *escola, FILE *stream);
void free_network(NETWORK *network);

#endif
