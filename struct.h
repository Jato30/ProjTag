#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct{
	int id;
	int habilitacoes;
	int preferencia[5];
} PROF;

typedef struct{
	int id;
	int exigencia;
	int vagas;
} ESCOLA;


typedef struct {
  int target;        // Index in the vertex[] array of neighboring vertex. Neighbor is the next discipline in the flow
  double weight;     // Weight of edge.  1 if no weight is specified.
} EDGE;

typedef struct {
  int id;            // GML ID number of vertex
  int degree;        // Degree of vertex (out-degree for directed nets)
  // char *label;       // GML label of vertex.  NULL if no label specified
  EDGE *edge;        // Array of EDGE structs, one for each neighbor
} VERTEX;

typedef struct {
  int nvertices;     // Number of vertices in network
  int directed;      // 1 = directed network, 0 = undirected
  VERTEX *vertex_p;    // Array of VERTEX structs, one for each vertex of type PROF
	VERTEX *vertex_e;    // Array of VERTEX structs, one for each vertex of type ESCOLA
} NETWORK;


typedef struct{
	PROF professor[100];
	ESCOLA escola[50];
	NETWORK grafo;
} PROF_ESC;

#endif // __STRUCT_H__
