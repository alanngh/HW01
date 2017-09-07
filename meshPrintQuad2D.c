#include "mesh.h"

void meshPrintQuad2D(mesh *msh){
	int e,v;

	printf("mesh print:\n");
	printf("Nelements = %d\n",msh->Nelements);
	printf("Nnodes = %d\n",msh->Nnodes);

	printf("\nnodes and its coordinates\n\n");

	for (v=0;v<msh->Nnodes;++v){
		printf("node coordinate %d maps to %lf %lf\n",
			v+1,msh->VX[v],msh->VY[v]);
	}

	printf("\nElement and its nodes\n\n");

	for (e=0;e<msh->Nelements;++e){
		printf("element %d maps to %d %d %d %d nodes\n",
			e+1,msh->EToV[e*msh->Nverts+0]+1, msh->EToV[e*msh->Nverts+1]+1, 
			msh->EToV[e*msh->Nverts+2]+1, msh->EToV[e*msh->Nverts+3]+1);
	}
	printf("\n\n");
}

