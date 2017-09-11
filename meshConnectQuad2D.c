#include "mesh.h"

typedef struct {
  int e;  // element face belongs to
  int f;  // local face number
  int v1; // vertex number of smallest vertex on face
  int v2; // vertex number of largest vertex on face
  int eN; // element neighbor number
  int fN; // element neighbor face number
}face;

// return -1 if face A is "less than" B
// return +1 if face A is "greater than" B
// else return 0 (face A and B match)
int compareFaces(const void *A, const void *B){

  const face *faceA = (face*) A;
  const face *faceB = (face*) B;

  const int v1a = faceA->v1;
  const int v2a = faceA->v2;
  const int v1b = faceB->v1;
  const int v2b = faceB->v2;

  // check most significant digit of hashes in base Nv
  if(v2a<v2b) return -1;
  if(v2a>v2b) return +1;

  /// check least significant digit of hash
  if(v1a<v1b) return -1;
  if(v1a>v1b) return +1;

  return 0;
  
}

// face = element+face pair
// return -1 if face A is "less than" B
// return +1 if face A is "greater than" B
// else return 0 (face A and B match)
int compareOrigins(const void *A, const void *B){

  const face *faceA = (face*) A;
  const face *faceB = (face*) B;

  const int ea = faceA->e;
  const int eb = faceB->e;
  const int fa = faceA->f;
  const int fb = faceB->f;

  // check most significant digit of hashes in base Nv
  if(ea<eb) return -1;
  if(ea>eb) return +1;

  /// check least significant digit of hash
  if(fa<fb) return -1;
  if(fa>fb) return +1;

  return 0;
  
}

// Print the information as the table on the lecture notes
void printFaces(face *fc,int m,int n){
	int i=0;
	// nodes start at 1 in gmesh
	// faces start at 0
	// -1 indicate no exist
	printf("\ne\tf\tv1\tv2\teN\tfN");
	for (i=0;i<m*n;++i){
		printf("\n%d\t%d\t%d\t%d\t%d\t%d",(fc+i)->e+1, (fc+i)->f,(fc+i)->v1+1,(fc+i)->v2+1,
			(fc+i)->eN>=0?(fc+i)->eN+1:(fc+i)->eN,(fc+i)->fN);
	}
	printf("\n\n");
}


// to do: implement comparison function to compare vertices and a function to compare element/face info
void meshConnectQuad2D(mesh *msh){

  // allocate an array of faces
  face *faces = (face*) calloc(msh->Nelements*msh->Nfaces, sizeof(face));
  int e, n, v1, v2;
  
  // loop over each element
  for(e=0;e<msh->Nelements;++e){
    int base = e*msh->Nfaces;
    
    // encode face 0 in the faces list
    faces[base].e = e;
    faces[base].f = 0;
    v1 = msh->EToV[e*msh->Nverts+0];
    v2 = msh->EToV[e*msh->Nverts+1];
    faces[base].v1 = mymin(v1,v2);
    faces[base].v2 = mymax(v1,v2);
    faces[base].eN = -1;
    faces[base].fN = -1;
    ++base;

    // encode face 1 in the faces list
    faces[base].e = e;
    faces[base].f = 1;
    v1 = msh->EToV[e*msh->Nverts+1];
    v2 = msh->EToV[e*msh->Nverts+2];
    faces[base].v1 = mymin(v1,v2);
    faces[base].v2 = mymax(v1,v2);
    faces[base].eN = -1;
    faces[base].fN = -1;
    ++base;

    // encode face 2 in the faces list
    faces[base].e = e;
    faces[base].f = 2;
    v1 = msh->EToV[e*msh->Nverts+2];
    v2 = msh->EToV[e*msh->Nverts+3];
    faces[base].v1 = mymin(v1,v2);
    faces[base].v2 = mymax(v1,v2);
    faces[base].eN = -1;
    faces[base].fN = -1;
    ++base;

	// encode face 3 in the faces list
    faces[base].e = e;
    faces[base].f = 3;
    v1 = msh->EToV[e*msh->Nverts+3];
    v2 = msh->EToV[e*msh->Nverts+0];
    faces[base].v1 = mymin(v1,v2);
    faces[base].v2 = mymax(v1,v2);
    faces[base].eN = -1;
    faces[base].fN = -1;
    ++base;

  }

	printf("\nInitial table\n");
	printFaces(faces,msh->Nelements,msh->Nfaces);
		
								       
  // sort by unique face hash
  qsort(faces, msh->Nelements*msh->Nfaces, sizeof(face), compareFaces);
  
	printf("\nAfter sort\n");
	printFaces(faces,msh->Nelements,msh->Nfaces);


  // loop through list of faces looking for a match
  for(n=0;n<msh->Nelements*msh->Nfaces-1;++n){
    // compare face n with face n+1 using the comparison function
    if(!compareFaces(faces+n, faces+n+1)){
      // found a match so add the face neighbor information
      faces[n].eN = faces[n+1].e;
      faces[n].fN = faces[n+1].f;
      faces[n+1].eN = faces[n].e;
      faces[n+1].fN = faces[n].f;
    }
   /* printf("e=%d, f=%d, eN=%d, fN=%d\n",
	   faces[n].e,
	   faces[n].f,
	   faces[n].eN,
	   faces[n].fN);*/
  }

	printf("\nAfter fill in info\n");
	printFaces(faces,msh->Nelements,msh->Nfaces);
  
	// sort faces back to their original order
  	qsort(faces, msh->Nelements*msh->Nfaces, sizeof(face), compareOrigins);

	printf("\nSort back to initial order\n");
	printFaces(faces,msh->Nelements,msh->Nfaces);

  
  
  // storage to store element-to-element connectivity
  msh->EToE = (int*) calloc(msh->Nelements*msh->Nfaces, sizeof(int));
  
  // storage to store element-to-local face connectivity
  msh->EToF = (int*) calloc(msh->Nelements*msh->Nfaces, sizeof(int));

  // to do: scrape the EToE and EToF data from the faces list
}


