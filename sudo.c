// Author: Ashish Acharya
// Date: Sept 29, 2021
// Operating Systems
// Project 1
// Sudoku Verifier using threads.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef int bool;
#define true 1
#define false 0

// Global Variables
int sudokuPuzzle[9][9];

bool colArray[9];
bool rowArrayl[9];
bool subgridArray[9];

int tid_column[9];
int tid_row[9];
int tid_subgrid[9];

void *colChecker(void *param);
void *rowChecker(void *param);
void *subgridChecker(void *param);


typedef struct cellRange{
	int tr; //topRow
	int br; //bottomRow
	int lc; //leftColumn
	int rc; //rightColumn;
} cellRange;

cellRange cols[9];
cellRange rows[9];
cellRange subgrids[9];

void *colChecker(void *param){
    int checkerArray[] = {0,0,0,0,0,0,0,0,0};
    int index = *(int*)param;

    tid_column[index] = pthread_self();

    for(int f = 0; f < 9; f++){
        int value = sudokuPuzzle[f][cols[index].lc];
        printf("%d\n", value);
        checkerArray[value-1] = 1;
    }

    colArray[index] = true;
    for(int f = 0; f < 9; f++){
        if(checkerArray[f]==0){
            colArray[index] = false;
        }
    }
    free(param);
}

/*
void *rowChecker(void *param){

}

void *subgridChecker(void *param){

}
*/

int main(){

	// Read a text file named SudokuPuzzle.txt
	//  x and y are loop counters
	FILE *in_file;
	int x, y;


	in_file =  fopen("SudokuPuzzle.txt","r");

	if (in_file == NULL){
		printf("Error! Could not open file\n");
		return(-1);
	}// endif

	//READ THE SUDOKU PUZZLE
	for (x=0; x<9; x++) {
		for (y=0; y<9; y++){
			fscanf(in_file, "%d", &sudokuPuzzle[x][y]);
		} //end for y
	}//end for  x
	fclose(in_file);

	//PRINT THE SUDOKU PUZZLE
	for (x=0; x<9; x++) {
		for (y=0; y<9; y++){
			printf("%d\t", sudokuPuzzle[x][y]);
		} //end for y
		printf("\n");
	}//end for  x



	//for (x=0; x<9; x++){
	//	b =  column_bool[x];
	//	//printf("%s ", b ? "true" : "false");
	//}//end for



	// INITIALIZE AND POPULATE column structs
	//     tr,br,lc,rc
	// c1 = {0,8,0,0}
	// c2 = {0,8,1,1}
	// c8 = {0,8,7,7}
	for (int i = 0; i < 9; i++){
		cols[i].tr = 0;
		cols[i].br = 8;
		cols[i].lc = i;
		cols[i].rc = i;
	}

	// INITIALIZE AND POPULATE row structs
	//     tr,br,lc,rc
	// r1 = {0,0,0,8}
	// r2 = {1,1,0,8}
	// r8 = {0,8,0,8}
	for (int i = 0; i < 9; i++){
		rows[i].tr = i;
		rows[i].br = i;
		rows[i].lc = 0;
		rows[i].rc = 8;
	}


	// INITIALIZE AND POPULATE subgrid structs
	//	tr,br,lc,rc
	// sg1 = {0,2,0,2}
	// sg2 = {0,2,3,5}
	// sg3 = {0,2,6,8}
	// sg4 = {3,5,0,2}
	// sg5 = {3,5,3,5}
	// sg6 = {3,5,6,8}
	// sg7 = {6,8,0,2}
	// sg8 = {6,8,3,5}
	// sg9 = {6,8,6,8}

	for (int i = 0; i < 9; i+=3){
		for(int j = 0; j <9; j += 3){
			subgrids[i].tr = i;
			subgrids[i].br = i+2;
			subgrids[i].lc = j;
			subgrids[i].rc = j + 2;
		}//end for j
	}//end for i

	pthread_t colThread[9];
	//pthread_t rowThread[9];
	//pthread_t subgridThread[9];
	int i;
	for (int i = 0; i < 9; i++){
		int* a = malloc(sizeof(int));

		*a = i;

		if(pthread_create(&colThread[i], NULL, colChecker, a)){
            perror("Failed to create row thread");
        }

	}//end for

    for(i = 0; i < 9; i++){
        if (pthread_join(colThread[i], NULL)) {
            perror("Failed to join column thread");
        }
    }

    /*
    for (int i = 0; i < 9; i++){
		int* a = malloc(sizeof(int));

		*a = i;

		pthread_create(&rowThread[i], NULL, rowChecker,  a);
	}//end for


	for (int i = 0; i < 9; i++){
		int* a = malloc(sizeof(int));

		*a = i;

		pthread_create(&subgridThread[i], NULL, subgridChecker, a);
	}//end for
    */

    for(int i = 0; i < 9; i++){
        if(colArray[i] == true){
            printf("Column: %d Valid!\n",tid_column[i]);
        } else {
            printf("Column: %d Invalid!\n",tid_column[i]);
        }
    }

	return 0;
}//end main
