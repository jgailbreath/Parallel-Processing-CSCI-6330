// This file is a first practice attempt at building a matrix multiplication program.
// Created by: Ryan Morse
// Class: CSCI 4330 - Parallel Processing Concepts
// Created on: 09/30/2022
// Last edit: 11/15/2022

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <cstdlib>
using namespace std;

// Function to allocate 2D arrays
double** initMat()
{
        int i, j, row=19, col=19;
        double** mat = (double**) malloc(row*sizeof(double*));

        for (i=0 ; i<row ; i++)
        {       mat[i] = (double*) malloc(col*sizeof(double));  }
	return mat;
}


int main()
{
	std::ifstream matFile;
	std::string hold;	// String is a temporary placeholder for values from the file
        double **initMat1, **initMat2, **finMat, **swp, tmp;
	double timeT;
	int i, j, k, steady, iter = 0;
	clock_t time;
	
	// Initiallizing Matrices
	initMat1 = initMat();
	initMat2 = initMat();
	finMat = initMat();
		
	matFile.open("./mat.csv", ios::in);
	
	// Loading matrices with values read from 'mat.csv'
	if (matFile)
        {
                for (i=0;i<19;i++)
                {
		       	for (j=0;j<19;j++)
                        {       
				getline(matFile,hold, ',');
				/* This line is an untested solution to \n characters in csv files
				 * If not handled the \n characters must first be stripped from the csv
				 */
				//if ((int)atoi(hold.c_str()) == (int)atoi("\n"))
				//{	getline(matFile, hold, ',');	}
				
				initMat1[i][j] = (double)atof(hold.c_str());
                                initMat2[i][j] = (double)atof(hold.c_str());
				//cout << initMat1[i][j] << " ,";	//Test print of initial matrix
			}
                //cout << endl;
		}
        }
        

        else
        {
                printf("'mat.csv' was not found");
                return 0;
        }
        
	matFile.close();

	time = clock();		// Start time
	
	// This is the matrix multiplication nested loop
	do
	{
		steady = 1;
		iter++;
		for (i=0;i<19;i++)
		{
			for (j=0;j<19;j++)
			{
				tmp = 0.0;
				for (k=0;k<19;k++)
				{       tmp += initMat1[i][k] * initMat2[k][j];       }
				finMat[i][j] = tmp;
				if ( j<12 && steady == true && finMat[i][j] > 0.0)
				{	steady = 0;		}
					
        		}
		}

		if (steady == 0)
		{	
			swp = finMat;
			finMat = initMat1;
			initMat1 = swp;
		}

	} while(steady != 1);
	

	time = clock()-time;
	timeT = ((double)time)/CLOCKS_PER_SEC;
	
	for (i=0;i<19;i++)
        {
                for (j=0;j<19;j++)
                {       printf("%f, ",finMat[i][j]);   }
                printf("\n");
        }
	
	printf("Time Total: %f\n",timeT);	
	return 0;
}
