#include "mesh.h"


int main(int argc, char**argv){

    //mesh *msh = meshReaderQuad2D(argv[argc-1]);
	mesh *msh = meshReaderQuad2D("Square.msh");
	meshPrintQuad2D(msh);

	// Node's index starts at 1 (Gmsh) so I printed them adding 1
	// Face's index starts at 0 and the numbering gous counter-clockwise
	
  	meshConnectQuad2D(msh);

	printf("\n");
	return 1;
}

