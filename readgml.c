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


// Function to allocate space for a network structure stored in a GML file
// and determine the parameters (id, label) of each of the vertices.

void montar_estrutura(PROF_ESC* G)
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
					G->professor[i].id = lido;
					lido = 0;
				}

				while(isdigit(line[j]) == 0){
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
					G->professor[i].habilitacoes = lido;
					lido = 0;
				}


				while(isdigit(line[j]) == 0){
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
						G->professor[i].preferencia[k] = lido;
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
			if(line[j] == '(' && line[j+1] == 'E'){
				j = 2;
				do{
					lido += (line[j] - '0');
					if(isdigit(line[j+1])){
						lido *= 10;
					}
					j++;
				}while(isdigit(line[j]));
				G->escola[i].id = lido;
				lido = 0;
			}

			while(isdigit(line[j]) == 0 && j < strlen(line)){
				j++;
			}
			if(j < strlen(line) &&   (line[j-3] == ')' && line[j-2] == ':' && line[j-1] == '(') ){
				G->escola[i].exigencia = (line[j] - '0');
				j++;
				lido = 0;
			}


			if(j < strlen(line) && j+1 < strlen(line) && (line[j] == ')' && line[j+1] == ':' && line[j+2] == '(') ){
				j += 3;
				G->escola[i].vagas = (line[j] - '0');
			}
			else{
				G->escola[i].vagas = 0;
			}
			j++;
		}


	}

}

void criar_grafo(PROF_ESC* G){
	G->grafo.directed = 0; // Não-direcionado
	G->grafo.nvertices = 150; // 100 professores + 50 escolas

	G->grafo.vertex_p = calloc(100, sizeof(VERTEX)); // 100 vertices do tipo PROF
	G->grafo.vertex_e = calloc(50, sizeof(VERTEX)); // 50 vertices do tipo ESCOLA



	// PREENCHE VERTICES DE ESCOLAS
	int i, j, k;
	int count_edges = 0;
	for(i = 0; i < 50; i++){
		G->grafo.vertex_e[i].id = G->escola[i].id;
		for(j=0; j < 100; j++){
			if(G->professor[j].habilitacoes >= G->escola[i].exigencia){
				count_edges++;
			}
		}
		G->grafo.vertex_e[i].edge = calloc(count_edges, sizeof(EDGE));
		G->grafo.vertex_e[i].degree = count_edges;
		count_edges = 0;
	}
	count_edges = 0;
	for(i = 0; i < 50; i++){
		for(j=0; j < 100; j++){
			if(G->professor[j].habilitacoes >= G->escola[i].exigencia){
				// Cria Aresta
				G->grafo.vertex_e[i].edge[count_edges].target = G->professor[j].id; // Note que target é o id do professor, e NÃO o índice no vetor

				// PREENCHE PESOS (As preferencias de escola dos professores são o peso)
				// Notação: Maior preferencia: degree = 1, menor preferencia degree = 5
				// Como não é realmente um degree, fiz assim para que 1 seja a primeira preferencia (pra ficar mais facil de emparelhar)
				for(k = 0; k < 5; k++){
					if(G->grafo.vertex_e[i].id == G->professor[j].preferencia[k]){
						G->grafo.vertex_e[i].edge[count_edges].weight = k+1;
					}
				}
				count_edges++;
			}
		}
		count_edges = 0;
	}



	// PREENCHE VERTICES DE PROFESSORES
	count_edges = 0;
	for(i = 0; i < 100; i++){
		G->grafo.vertex_p[i].id = G->professor[i].id;
		for(j=0; j < 50; j++){
			if(G->professor[i].habilitacoes >= G->escola[j].exigencia){
				count_edges++;
			}
		}
		G->grafo.vertex_p[i].edge = calloc(count_edges, sizeof(EDGE));
		G->grafo.vertex_p[i].degree = count_edges;
		count_edges = 0;
	}

	count_edges = 0;
	for(i = 0; i < 100; i++){
		for(j=0; j < 50; j++){
			if(G->professor[i].habilitacoes >= G->escola[j].exigencia){
				G->grafo.vertex_p[i].edge[count_edges].target = G->escola[j].id;

				// PREENCHE PESOS (As preferencias de escola dos professores são o peso)
				// Notação: Maior preferencia: degree = 1, menor preferencia degree = 5
				// Como não é realmente um degree, fiz assim para que 1 seja a primeira preferencia (pra ficar mais facil de emparelhar)
				for(k = 0; k < 5; k++){
					if(G->grafo.vertex_p[i].edge[count_edges].target == G->professor[j].preferencia[k]){
						G->grafo.vertex_p[i].edge[count_edges].weight = k+1;
					}
				}
				count_edges++;
			}
		}
		count_edges = 0;
	}

}

// Function to read a complete network

int read_prof_escola(PROF_ESC* G, FILE *stream)
{
	fill_buffer(stream);
	montar_estrutura(G);
	criar_grafo(G);
	// get_degrees(network);
	// read_edges(network);
	free_buffer();

	return 0;
}


// Function to free the memory used by a network again

void free_memo(PROF_ESC *G)
{
	int i;

	for (i=0; i<100; i++) {
		free(G->professor[i].preferencia);
	}
	free(G->professor);
	free(G->escola);

	for (i=0; i<100; i++) {
		free(G->grafo.vertex_p[i].edge);
		if(i<50){
			free(G->grafo.vertex_e[i].edge);
		}
	}
	free(G->grafo.vertex_p);
	free(G->grafo.vertex_e);
}

