/*-----------------------------------------------------------------------------------------------------+
| pg1.c                  |Graph analyzer for AED-LAB5 (LAB6)                       		       |
|                        |Graph Density, Branch and Degree calculator.                                 |
|                        |                                                                             |
+------------------------------------------------------------------------------------------------------+
| Authors: Joao Barreiros C. Rodrigues nº99968, Henrique Ramos Delfino nº99957                         |
|          LEEC-IST                                                                                    |
| Date: 31 October 2021                                                                                |
+-----------------------------------------------------------------------------------------------------*/


#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
/*Function Name: matrixalloc
  Input: 2 integers (number of collumns and lines)
  Output: pointer to pointer to int
  Date Created: 04 Out 2021
  Last Revised: 05 Out 2021
  Definition: Alloc and initialize 2D matrix with the given values for colummns and lines
  Based on: https://phyweb.physics.nus.edu.sg/~phywjs/CZ1102/lecture20/tsld014.htm
*/
int** matrixalloc(int size){
	int** matrix = NULL;
	int aux = 0;
	matrix = (int**)calloc(size,sizeof(int*)); /*alloc and init number of lines (number of pointers to pointers)*/
	for(aux=0; aux < size; aux++)
		matrix[aux]=(int*)calloc(size,sizeof(int));
	return matrix;
}

/*Function Name: freematrix
  Input: Matrix (pointer to pointer to int), 2 integers (number of lines and colummns)
  Output: No Return. Free'd matrix related blocks from Heap
  Date Created: 31 Oct 2021
  Last Revised:
  Definition: free allocated matrix completly
*/
void freematrix (int** matrix, int nodes){
	int aux = 0;
	for(aux=0; aux < nodes; aux++){ 
		free(matrix[aux]);
	}
	free(matrix);
}
char* gen_outname(char* _filenamein){
	int lenght=strlen(_filenamein);
	char* _filenameout=(char*)malloc((sizeof(char)*lenght)+2);
	strcpy(_filenameout, _filenamein);
	_filenameout[lenght-3]='e';
	_filenameout[lenght-2]='d';
	_filenameout[lenght-1]='g';
	_filenameout[lenght]='e';
	_filenameout[lenght+1]='\0';
	return _filenameout;
}
void iofile(char* _filenamein){
	int nodes=0, seed=0, auxline=0, auxcol=0, branches=0, degree=0,auxdegree=0, examinedcol=0;
	int** matrix=NULL;
	FILE* fp=fopen(_filenamein,"r");
	char* _filenameout=NULL;
	float density=0;
        FILE* fpout=NULL;
	if(fscanf(fp,"%d", &nodes )!= 1)
		exit(1);
	matrix=matrixalloc(nodes);
	for (auxline=0; auxline < nodes; auxline++){
		for(auxcol=0; auxcol < nodes; auxcol++){
			if(fscanf(fp,"%d", &seed )!= 1)
                		exit(1);
			matrix[auxline][auxcol]=seed;
		}
	}

        _filenameout=gen_outname(_filenamein);
        fpout=fopen(_filenameout, "w");
	for (auxline=0; auxline < nodes; auxline++){  /*count branches*/
                for(auxcol=examinedcol; auxcol < nodes; auxcol++){
                        if(matrix[auxline][auxcol]!=0)
                                branches++;
                }
		examinedcol++; /*as the matrix is simetric we can discard the X col after examining X line*/
        }
	examinedcol=0;
        density=(float) 2*(branches/nodes);
        for (auxline=0; auxline < nodes; auxline++){  /*count branches*/
                for(auxcol=examinedcol; auxcol < nodes; auxcol++){
                        if(matrix[auxline][auxcol]!=0)
                                auxdegree++;
                }
                examinedcol++; /*as the matrix is simetric we can discard the X col after examining X line*/
		if(auxdegree > degree)
			degree=auxdegree;
        }
        printf("Matrix Degree: %d \n Matrix Density: %f \n",degree, density);
	fprintf(fpout,"%d %d \n", nodes, branches);
	 for (auxline=0; auxline < nodes; auxline++){
                for(auxcol=examinedcol; auxcol < nodes; auxcol++){
                        if(matrix[auxline][auxcol]!=0)
                        	fprintf(fpout,"[%d]-[%d]: %d \n", auxline+1, auxcol+1, matrix[auxline][auxcol]);
                }
		examinedcol++;
        }
	freematrix(matrix, nodes);
}

int main(int argc, char *argv[]){
	char  *_filenamein = argv[1];
    	iofile(_filenamein);
	return 0;
}    
