#include "mesh.h"

mesh *meshReaderQuad2D(char *fileName){

	mesh *msh =(mesh*) calloc(1,sizeof(mesh));

	FILE *mshFile = fopen(fileName,"r");
 	char buf[BUFSIZ];
	int v, e;

	if (mshFile==NULL){
		printf("mesh file %s did not open, exiting\n",fileName);
		exit(-1);
	}

	// hard code for quads
	msh->Nverts =4;
	msh->Nfaces =4;

	do {
		fgets(buf,BUFSIZ,mshFile);
	} while(!strstr(buf,"$Nodes"));

	// read in number of vertices
	fgets(buf,BUFSIZ,mshFile);
	sscanf(buf,"%d",&(msh->Nnodes));

	// red in vertex coordinates
	msh->VX = (dfloat*) calloc(msh->Nnodes,sizeof(dfloat));
	msh->VY = (dfloat*) calloc(msh->Nnodes,sizeof(dfloat));

	for (v=0; v<msh->Nnodes;++v){
        fgets(buf,BUFSIZ,mshFile);
        sscanf(buf,"%*d%lf%lf",msh->VX+v, msh->VY+v);
	}

	do {
		fgets(buf,BUFSIZ,mshFile);
	} while(!strstr(buf,"$Elements"));

	// read in number of elements
	fgets(buf,BUFSIZ,mshFile);
	sscanf(buf,"%d",&(msh->Nelements));

	// red in element to vertex connectivity
	msh->EToV = (int*) calloc(msh->Nelements*msh->Nnodes,sizeof(int));

	for (e=0; e<msh->Nelements;++e){
        fgets(buf,BUFSIZ,mshFile);
        sscanf(buf,"%*d%*d%*d%*d%*d %d%d%d%d", msh->EToV + e*msh->Nverts+0,
               msh->EToV + e*msh->Nverts+1, msh->EToV + e*msh->Nverts+2,msh->EToV + e*msh->Nverts+3);

        // shift to zero indexing
        --(msh->EToV[e*msh->Nverts+0]);
        --(msh->EToV[e*msh->Nverts+1]);
        --(msh->EToV[e*msh->Nverts+2]);
		--(msh->EToV[e*msh->Nverts+3]);
	}

	fclose(mshFile);

	// Test Clockwisely
	int a,b,c,d , cond;
	float A1, A2, temp;

	for (e=0;e<msh->Nelements;++e){
		// index stars at 0 in C but at 1 in Gmsh
		a = msh->EToV[e*msh->Nverts+0];
	 	b = msh->EToV[e*msh->Nverts+1];
		c = msh->EToV[e*msh->Nverts+2];
		d = msh->EToV[e*msh->Nverts+3];
		printf("\nElement %d with nodes a=%d b=%d c=%d d=%d\n",e+1,a+1,b+1,c+1,d+1);
		printf("coordinates for node a =  %d  are  ( %f , %f )\n",a+1, msh->VX[a],msh->VY[a]);
		printf("coordinates for node b =  %d  are  ( %f , %f )\n",b+1, msh->VX[b],msh->VY[b]);
		printf("coordinates for node c =  %d  are  ( %f , %f )\n",c+1, msh->VX[c],msh->VY[c]);
		printf("coordinates for node d =  %d  are  ( %f , %f )\n",d+1, msh->VX[d],msh->VY[d]);

		cond=0;

		do{
			A1 = (msh->VX[a]*msh->VY[b] + msh->VX[b]*msh->VY[c] + msh->VX[c]*msh->VY[a]) -(msh->VX[c]*msh->VY[b] + msh->VX[a]*msh->VY[c] + msh->VX[b]*msh->VY[a]); 
			A2 = (msh->VX[a]*msh->VY[c] + msh->VX[c]*msh->VY[d] + msh->VX[d]*msh->VY[a]) -(msh->VX[d]*msh->VY[c] + msh->VX[a]*msh->VY[d] + msh->VX[c]*msh->VY[a]);

			printf("A1  =  %f  and   A2 = %f \n",A1,A2);
			printf("Order : %d  %d   %d   %d \n",a+1,b+1,c+1,d+1);

			if (A1 >0 && A2 > 0) cond =1;

			if (A1 < 0){
				temp = b; b = c; c = temp;
			}

			if (A2 < 0){
				temp = c; c = d; d = temp;
			}
		} while (cond != 1);
	} 

	return msh;
}


