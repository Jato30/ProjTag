// Inclusions

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "struct.h"


// Constants

#define LINELENGTH 100

// Types

typedef struct line {
	char *str;
	struct line *ptr;
} LINE;

// Globals

LINE *first;
LINE *current;


// Function to read one line from a specified stream.  Return value is
// 1 if an EOF was encountered.  Otherwise 0.

int read_line(FILE *stream, char line[LINELENGTH])
{
	if (fgets(line,LINELENGTH,stream)==NULL) return 1;
	line[strlen(line)-1] = '\0';   // Erase the terminating NEWLINE
	return 0;
}


// Function to read in the whole file into a linked-list buffer, so that we
// can do several passes on it, as required to read the GML format
// efficiently

int fill_buffer(FILE *stream)
{
	int length;
	char line[LINELENGTH];
	LINE *previous;

	if (read_line(stream,line)!=0) {
		first = NULL;                // Indicates empty buffer
		return 1;
	}
	length = strlen(line) + 1;
	first = malloc(sizeof(LINE));
	first->str = malloc(length*sizeof(char));
	strcpy(first->str,line);

	previous = first;
	while (read_line(stream,line)==0) {
		length = strlen(line) + 1;
		previous->ptr = malloc(sizeof(LINE));
		previous = previous->ptr;
		previous->str = malloc(length*sizeof(char));
		strcpy(previous->str,line);
	}
	previous->ptr = NULL;          // Indicates last line

	return 0;
}


// Function to free up the buffer again

void free_buffer()
{
	LINE *thisptr;
	LINE *nextptr;

	thisptr = first;
	while (thisptr!=NULL) {
		nextptr = thisptr->ptr;
		free(thisptr->str);
		free(thisptr);
		thisptr = nextptr;
	}
}


// Function to reset to the start of the buffer again

void reset_buffer()
{
	current = first;
}


// Function to get the next line in the buffer.  Returns 0 if there was
// a line or 1 if we've reached the end of the buffer.

int next_line(char line[LINELENGTH])
{
	if (current==NULL) return 1;
	strcpy(line,current->str);
	current = current->ptr;
	return 0;
}

// Function to count the vertices in a GML file.  Returns number of vertices.

int count_professores()
{
	int result=0;
	char *ptr;
	char line[LINELENGTH];

	reset_buffer();

	while (next_line(line)==0) {
		ptr = strstr(line,"(P");
		if (ptr!=NULL) {
			result++;
		}
	}

	return result;
}

int count_escolas()
{
	int result=0;
	char *ptr;
	char line[LINELENGTH];

	reset_buffer();

	while (next_line(line)==0) {
		ptr = strstr(line,"):(");
		if (ptr!=NULL) {
			result++;
		}
	}

	return result;
}


// Function to compare the IDs of two vertices

// int cmpid(VERTEX *v1p, VERTEX *v2p)
// {
// 	if (v1p->id>v2p->id) return 1;
// 	if (v1p->id<v2p->id) return -1;
// 	return 0;
// }


// Function to allocate space for a network structure stored in a GML file
// and determine the parameters (id, label) of each of the vertices.

void create_network(NETWORK* G)
{
	int i;
	int length;
	char *ptr;
	char *start,*stop;
	char line[LINELENGTH];
	char label[LINELENGTH];


	// Go through the file reading the details of each vertex one by one

	reset_buffer();
	int qtd_prof = count_professores();
	reset_buffer();
	for (i=0; i<qtd_prof; i++) {

		// Skip to next "node" entry

		do {
			next_line(line);
		} while (strstr(line,"(P")==NULL);


		// Read in the details of this vertex


			// Look for ID

		ptr = strstr(line,"(P");
		if (ptr!=NULL){

			int lido = 0;
			int j = 0;
			while(j < strlen(line)){
				if(line[j] == '(' && line[j+1] == 'P'){
					j = 2;
					do{
						lido += (line[j] - '0');
						if(isdigit(line[j+1])){
							lido *= 10;
						}
						j++;
					}while(isdigit(line[j]));
					G->professor->id = lido;
					lido = 0;
				}

				while(!isdigit(line[j])){
					j++;
				}
				if(line[j-2] == ',' && line[j-1] == ' '){
					do{
						lido += (line[j] - '0');
						if(isdigit(line[j+1])){
							lido *= 10;
						}
						j++;
					}while(isdigit(line[j]));
					G->professor->habilitacoes = lido;
					lido = 0;
				}


				while(!isdigit(line[j])){
					j++;
				}
				if(line[j-5] == ')' && line[j-4] == ':' && line[j-3] == ' ' && line[j-2] == '(' && line[j-1] == 'E'){
					int k;
					for(k = 0; k < 5; k++){
						do{
							lido += (line[j] - '0');
							if(isdigit(line[j+1])){
								lido *= 10;
							}
							j++;
						}while(isdigit(line[j]));
						G->professor->preferencia[k] = lido;
						lido = 0;
						j += 3;
					}
				}

			}
		}

	}

	reset_buffer();
	int qtd_escola = count_escolas();
	reset_buffer();
	for (i=0; i<qtd_escola; i++) {

		// Skip to next "node" entry

		do {
			next_line(line);
		} while (strstr(line,"):(")==NULL);

		// Read in the details of this vertex


			// Look for ID

		ptr = strstr(line,"):(");
		if (ptr!=NULL){

			int lido = 0;
			int j = 0;
			while(j < strlen(line)){
				if(line[j] == '(' && line[j+1] == 'E'){
					j = 2;
					do{
						lido += (line[j] - '0');
						if(isdigit(line[j+1])){
							lido *= 10;
						}
						j++;
					}while(isdigit(line[j]));
					G->escola->id = lido;
					lido = 0;
				}

				while(!isdigit(line[j])){
					j++;
				}
				if(line[j-3] == ')' && line[j-2] == ':' && line[j-1] == '('){
					G->escola->exigencia = (line[j] - '0');
					j++;
					lido = 0;
				}


				if(line[j] == ')' && strlen(line) < j+1 && line[j+1] == ':' && line[j+2] == '('){
					j += 3;
					G->escola->vagas = (line[j] - '0');
				}
				else{
					G->escola->vagas = 0;
				}
			}
		}


	}

	// Sort the vertices in increasing order of their IDs so we can find them
	// quickly later

	// qsort(network->vertex,network->nvertices,sizeof(VERTEX),(void*)cmpid);
}


// Function to find a vertex with a specified ID using binary search.
// Returns the element in the vertex[] array holding the vertex in question,
// or -1 if no vertex was found.

// int find_vertex(int id, NETWORK *network)
// {
// 	int top,bottom,split;
// 	int idsplit;

// 	top = network->nvertices;
// 	if (top<1) return -1;
// 	bottom = 0;
// 	split = top/2;

// 	do {
// 		idsplit = network->vertex[split].id;
// 		if (id>idsplit) {
// 			bottom = split + 1;
// 			split = (top+bottom)/2;
// 		} else if (id<idsplit) {
// 			top = split;
// 			split = (top+bottom)/2;
// 		} else return split;
// 	} while (top>bottom);

// 	return -1;
// }
    

// Function to determine the degrees of all the vertices by going through
// the edge data

// void get_degrees(NETWORK *network)
// {
// 	int s,t;
// 	int vs,vt;
// 	char *ptr;
// 	char line[LINELENGTH];

// 	reset_buffer();

// 	while (next_line(line)==0) {

// 		// Find the next edge entry

// 		ptr = strstr(line,"edge");
// 		if (ptr==NULL) continue;

// 		// Read the source and target of the edge

// 		s = t = -1;

// 		do {

// 			ptr = strstr(line,"source");
// 			if (ptr!=NULL) sscanf(ptr,"source %i",&s);
// 			ptr = strstr(line,"target");
// 			if (ptr!=NULL) sscanf(ptr,"target %i",&t);

// 			// If we see a closing square bracket we are done

// 			if (strstr(line,"]")!=NULL) break;

// 		} while (next_line(line)==0);

// 		// Increment the degrees of the appropriate vertex or vertices

// 		if ((s>=0)&&(t>=0)) {
// 			vs = find_vertex(s,network);
// 			network->vertex[vs].degree++;
// 			if (network->directed==0) {
// 				vt = find_vertex(t,network);
// 				network->vertex[vt].degree++;
// 			}
// 		}

// 	}

//   return;
// }


// Function to read in the edges

// void read_edges(NETWORK *network)
// {
// 	int i;
// 	int s,t;
// 	int vs,vt;
// 	int *count;
// 	double w;
// 	char *ptr;
// 	char line[LINELENGTH];

// 	// Malloc space for the edges and temporary space for the edge counts
// 	// at each vertex

// 	for (i=0; i<network->nvertices; i++) {
// 		network->vertex[i].edge = malloc(network->vertex[i].degree*sizeof(EDGE));
// 	}
// 	count = calloc(network->nvertices,sizeof(int));

// 	// Read in the data

// 	reset_buffer();

// 	while (next_line(line)==0) {

// 		// Find the next edge entry

// 		ptr = strstr(line,"edge");
// 		if (ptr==NULL) continue;

// 		// Read the source and target of the edge and the edge weight

// 		s = t = -1;
// 		w = 1.0;

// 		do {

// 			ptr = strstr(line,"source");
// 			if (ptr!=NULL) sscanf(ptr,"source %i",&s);
// 			ptr = strstr(line,"target");
// 			if (ptr!=NULL) sscanf(ptr,"target %i",&t);
// 			ptr = strstr(line,"value");
// 			if (ptr!=NULL) sscanf(ptr,"value %lf",&w);

// 			// If we see a closing square bracket we are done

// 			if (strstr(line,"]")!=NULL) break;

// 		} while (next_line(line)==0);

// 		// Add these edges to the appropriate vertices

// 		if ((s>=0)&&(t>=0)) {
// 			vs = find_vertex(s,network);
// 			vt = find_vertex(t,network);
// 			network->vertex[vs].edge[count[vs]].target = vt;
// 			network->vertex[vs].edge[count[vs]].weight = w;
// 			count[vs]++;
// 			if (network->directed==0) {
// 			network->vertex[vt].edge[count[vt]].target = vs;
// 			network->vertex[vt].edge[count[vt]].weight = w;
// 			count[vt]++;
// 			}
// 		}

// 	}

// 	free(count);
// 	return;
// }


// Function to read a complete network

int read_prof_escola(NETWORK* G, FILE *stream)
{
	fill_buffer(stream);
	create_network(G);
	// get_degrees(network);
	// read_edges(network);
	free_buffer();

	return 0;
}


// Function to free the memory used by a network again

void free_network(NETWORK *network)
{
	int i;

	for (i=0; i<100; i++) {
		free(network->professor[i].preferencia);
	}
	free(network->professor);
	free(network->escola);
}
