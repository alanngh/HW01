#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dfloat double

typedef struct {
  int Nelements; // triangle count for mesh
  int Nnodes; // node count for mesh

  int Nverts; // number of nodes in each element
  int Nfaces;
  
  dfloat *VX; // x-coordinates of vertices in mesh
  dfloat *VY; // y-coordinates of vertices in mesh

  int *EToV;  // element-to-vertex connectivity array
  int *EToE;  // element-to-element connectivity array
  int *EToF;  // element-to-local face connectivity array
} mesh;

mesh *meshReaderQuad2D(char *fileName);

void meshPrintQuad2D(mesh *msh);

void meshConnectQuad2D(mesh *msh);

#define mymax(a,b) (((a)>(b))?(a):(b))
#define mymin(a,b) (((a)<(b))?(a):(b))
