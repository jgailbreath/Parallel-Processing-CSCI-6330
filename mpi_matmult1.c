/*  Course:  Parallel Processing Concepts-CSCI-6330
    Professor:  Dr. Khem N. Poudel
    Program:  Fall 2022: Semester Project-Matrix Multiplication Using MPI
    Developers:  J. Gailbreath, R. Morse, E. White. J. Long, D. Shamsieva
    Description:  This program uses MPI to create a parallel implementation that 
                  finds the steady state of an absorbing Markhov Chain.    
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define SIZE 19               // used to determine the size of rows and columns in matrices
#define MAGIC_NUMBER 0.00001  // value used as cutoff for matrix evaluation
#define TOP_LEFT 12           // value used to identify top left section of matrix--needed to determine steady state
#define MAIN 0                //  Identifier for main process
#define MAIN_OUT 1            //  Identifier for message type from main process
#define WORKER_OUT 2          //  Identifier for message type from worker process
#define GETNAME(var) #var     //  used to grab a variable name so it can be displayed

MPI_Status status;

int cycles = 0,         //  how many MPI instantiations did it take to reach steady state
	numtasks,           // number of tasks (size)
   	taskid,             //  task identifier
   	numworkers,         //number of worker tasks
   	source,             // task id of message source
   	dest,               //  task id of message destination
	msgtag,             //  message type (integer value to identify where the message is coming from)
	rc,                 //  Used to report error codes
	extra,              //  number of "extra" rows that did not evenly divide into chunks
	avg,                //  average number of rows for each worker
	offset,             //  used with chunk to determine bounds for rows processed by workers
   	chunk,              // "chunk size" of actual number rows being sent to worker thread
	flip = 2;           //  used to identify which matrix is being multiplied with the pitcher profile


double start, finish;   // for time tracking purposes
double OG[SIZE][SIZE],  //  matrices required for program
       M1[SIZE][SIZE],
       M2[SIZE][SIZE];  

double elapsed_time = 0;//  used to track time for MPI functions

bool flag = false;      // Used as sentinel value to end multiplication process

char* BTR_FILE;         //  for reading in batter profile
char* PCHR_FILE;        // for reading in pitcher profile

void matmult(int size, double a[][size], double b[][size], double c[][size]);  // function will perform matrix multiplication
void csv_to_matrix(int size, double m[][size], char* file);                    //  function will read in a CSV file and insert contents into a 2D-array
void copy_matrix(int size, double a[][size], double b[][size]);                //  function will copy contents of a 2D-array into a separate 2D-array
void display(int size, double a[][size]);                                      //  function will display contents of a 2D-array


int main(int argc, char* argv[]){
		MPI_Init(&argc, &argv);  //Initialize MPI opetations
		MPI_Comm_rank(MPI_COMM_WORLD, &taskid);  //Get the rank
		MPI_Comm_size(MPI_COMM_WORLD, &numtasks);  //Get number of processes
		if (numtasks < 2 ){
				printf("Need at least two MPI tasks. Quitting...\n");
				MPI_Abort(MPI_COMM_WORLD, rc);
				exit(1);
		}
		//  Main Thread
		if(taskid == MAIN){
			PCHR_FILE = "jAltuve_actual_19X19.csv";
			BTR_FILE = "mBumgarner_actual_19X19.csv";
			csv_to_matrix(SIZE, OG, PCHR_FILE);
			csv_to_matrix(SIZE, M1, PCHR_FILE);
			numworkers = numtasks - 1;
			extra = (SIZE % numworkers);
			avg = (SIZE / numworkers);

			numworkers = (numtasks - 1);
			printf("Number of tasks available= %d\n", numtasks);
			printf("Number of workers that will be assigned= %d\n", numworkers);
			//sleep(1);
			//display(SIZE, OG);
			//sleep(1);
			//display(SIZE, M1);
			//sleep(1);
			//display(SIZE, M2);
			printf("Extra = %d\n", extra);
			printf("AVG = %d\n", avg);
        }
		for(int z=0; z<500; z++){
                if(taskid == MAIN){
						msgtag = MAIN_OUT;
						offset = 0;
						start = MPI_Wtime();
						//  Sending chunks to be processed by worker threads
						for(dest= 1; dest<=numworkers; dest++){  // Identifying each worker outside of the main thread
                            chunk = (dest<=extra) ? avg+1 : avg;
								printf("DEST= %d ---EXTRA= %d ---OFFSET= %d ---CHUNK = %d\n", dest, extra, offset, chunk);
								//sleep(2);
								MPI_Send(&offset, 1, MPI_INT, dest, msgtag, MPI_COMM_WORLD);
								MPI_Send(&chunk, 1, MPI_INT, dest, msgtag, MPI_COMM_WORLD);
								MPI_Send(&OG[offset][0], (chunk*SIZE), MPI_DOUBLE, dest, msgtag, MPI_COMM_WORLD);
								MPI_Send(&M1, (SIZE*SIZE), MPI_DOUBLE, dest, msgtag, MPI_COMM_WORLD);
								offset = offset + chunk;
						}
						//  Receiving results from the worker threads
						msgtag = WORKER_OUT;
						for(source=1; source<=numworkers; source++){
								MPI_Recv(&offset, 1, MPI_INT, source, msgtag, MPI_COMM_WORLD, &status);
								MPI_Recv(&chunk, 1, MPI_INT, source, msgtag, MPI_COMM_WORLD, &status);
								MPI_Recv(&M2[offset][0], (SIZE*SIZE), MPI_DOUBLE, source, msgtag, MPI_COMM_WORLD, &status);
						}
                }
                if (taskid == MAIN)
                {
                    cycles++;
                    printf("Cycles: %d\n", cycles);
                }
                if(taskid == MAIN){
                    copy_matrix(SIZE, M1, M2);
                    printf("M1 updated.\n");
                }                          
                if(taskid == MAIN){
                    flag = true;
                    for(int i=0; i<TOP_LEFT; i++){
                        for(int j=0; j<TOP_LEFT; j++){
                            if(M1[i][j]>MAGIC_NUMBER){
                                flag = false;
                                i= TOP_LEFT+1;
								j= TOP_LEFT+1;
							}
						}
                    }
                    if(flag == true){
                        z=501;
                    }                           
                }                        
                if(taskid > MAIN){
                        msgtag = MAIN_OUT;
                        MPI_Recv(&offset, 1, MPI_INT, MAIN, msgtag, MPI_COMM_WORLD, &status);
                        MPI_Recv(&chunk, 1, MPI_INT, MAIN, msgtag, MPI_COMM_WORLD, &status);
                        MPI_Recv(&OG, (chunk*SIZE), MPI_DOUBLE, MAIN, msgtag, MPI_COMM_WORLD, &status);
                        MPI_Recv(&M1, (SIZE*SIZE), MPI_DOUBLE, MAIN, msgtag, MPI_COMM_WORLD, &status);
						for(int i=0; i<SIZE; i++){  // this represents the number of columns in the 2nd matrix
								for(int j=0; j<chunk; j++){  //  this represents the chunk size for the task (number of rows to process)
										M2[j][i] = 0.0;  //  making sure the value stored is zero
										for(int k=0; k<SIZE; k++){  //  this represents the columns in the 1st matrix
												M2[j][i] = M2[j][i] + (OG[j][k] * M1[k][i]);
										}
                                        //printf("ID= %d -- OFFEST= %d --M2[%d][%d]= %lf\n", taskid, offset, j, i, M2[j][i]);
								}
						}
						printf("ID %d completed a chunk.\n", taskid);
                        msgtag = WORKER_OUT;
                        MPI_Send(&offset, 1, MPI_INT, MAIN, msgtag, MPI_COMM_WORLD);
                        MPI_Send(&chunk, 1, MPI_INT, MAIN, msgtag, MPI_COMM_WORLD);
                        MPI_Send(&M2, (chunk*SIZE), MPI_DOUBLE, MAIN, msgtag, MPI_COMM_WORLD);
                }
                if(taskid == MAIN){
                    finish = MPI_Wtime();
                    elapsed_time = elapsed_time + (double)finish - (double)start;
                }
        }
    if(taskid == MAIN){
        printf("Run Time using MPI: %lf\n", elapsed_time);
		display(SIZE, M1);
    }        
    MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
										
void matmult(int size, double a[][size], double b[][size], double c[][size]){
		int i, j, k;
		for(i=0; i<size; i++){
				for(j=0; j<size; j++){
						c[i][j] = 0;
						for(k=0; k<size; k++){
								c[i][j] = c[i][j] + (a[i][k] * b[k][j]);
						}
				}
		}
}

void csv_to_matrix(int size, double m[][size], char* file){
		int i=0, j=0;
		char file_row[4096];
		FILE* fp;
		char* tok;
		fp = fopen(file, "r");
		if(fp == NULL){
				perror("Error opening file.\n");
				exit (1);
		}
		else{
				printf("File opened successfully.\n");
		}
		while(fgets(file_row, 4096, fp)){
				tok = strtok(file_row, ",");
				while(tok != NULL){
						m[i][j] = atof(tok);
						tok = strtok(NULL, ",");
						++j;
				}
				free(tok);
				i++;
				j = 0;
		}
		free(fp);
}

void copy_matrix(int size, double a[][size], double b[][size]){
		int i, j;
		for(i=0; i<size; i++){
				for(j=0; j<size; j++){
						a[i][j] = b[i][j];
				}
		}
}

void display(int size, double a[][size]){
		 int i, j;
		 char s[] = GETNAME(a);
         printf("MATRIX: %s\n", s);
         for(i=0; i<size; i++){
                 for(j=0; j<size; j++){
                         printf("%2.4lf\t", a[i][j]);
                 }
                 printf("\n");
         }
         printf("\n");
}


