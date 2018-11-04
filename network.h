// Header file for VERTEX, EDGE, and NETWORK data structures
//
// Mark Newman  11 AUG 06

#ifndef _NETWORK_H
#define _NETWORK_H

typedef struct {
  int target;        // Index in the vertex[] array of neighboring vertex. Neighbor is the next discipline in the flow
  double weight;     // Weight of edge.  1 if no weight is specified.
} EDGE;

typedef struct {
  int id;            // GML ID number of vertex
  int degree;        // Degree of vertex (out-degree for directed nets)
  char *label;       // GML label of vertex.  NULL if no label specified
  EDGE *edge;        // Array of EDGE structs, one for each neighbor
} VERTEX;

typedef struct {
  int nvertices;     // Number of vertices in network
  int directed;      // 1 = directed network, 0 = undirected
  VERTEX *vertex;    // Array of VERTEX structs, one for each vertex
} NETWORKss;

#endif
