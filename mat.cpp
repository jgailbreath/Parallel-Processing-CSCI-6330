// This file is a first practice attempt at building a matrix multiplication program.
// Created by: Ryan Morse
// Class: CSCI 4330 - Parallel Processing Concepts
// Created on: 09/30/2022
// Last edit: 10/02/2022

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <cstdlib>
using namespace std;

// Function to round off doubles to significant digits
double roundoff(double value, unsigned char prec)
{
  double pow_10 = pow(10.0f, (double)prec);
  return round(value * pow_10) / pow_10;
}

/* The intention here is to read all the values from the csv first and create 2 matrices loaded with the same values.
 * That part works, and we get 2 equal matrices loaded with doubles from the csv rounded to 3 digits.
 * The trouble is in the matrix mutliplication. Somewhere either in the multiplication or addition operations a loss
 * of precision is causing wild values to pop up in the final matrix. The rounding was intended to prevent this, but dooesn't work.*/
int main()
{
        std::ifstream matFile;
        std::string hold;       // String is a temporary placeholder for values from the file
        double initMat1[19][19], initMat2[19][19], finMat[19][19];
        double timeT;
        int i, j, k;
        clock_t time;
        bool steady = false;

        matFile.open("./mat.csv", ios::in);

        if (matFile)
        {
                for (i=0;i<19;i++)
                {
                        for (j=0;j<19;j++)
                        {
                                getline(matFile,hold, ',');
                                /* This line is an untested solution to \n characters in csv files
                                 * If not handled the \n characters must first be stripped from the csv
                                 * 
                                if ((int)atoi(hold.c_str()) == (int)atoi("\n"))
                                {       getline(matFile, hold, ','      }
                                */
                                initMat1[i][j] = roundoff((double)atof(hold.c_str()), 3);
                                initMat2[i][j] = roundoff((double)atof(hold.c_str()), 3);
                                //printf("%f ,", initMat1[i][j]);       //Test print of initial matrix
                        }
                //printf("\n");
                }
        }


        else
        {
                printf("'mat.csv' was not found");
                return 0;
        }

        matFile.close();

        time = clock();         // Start time

        // This is the matrix multiplication nested loop
        for (i=0;i<19;i++)
        {
                for (j=0;j<19;j++)
                {
                        for (k=0;k<19;k++)
                        {       finMat[i][j] = roundoff(finMat[i][j]+(initMat1[i][k]*initMat2[k][j]),3);       }
                if (i<12 && j<7 && steady == true &&finMat[i][j] > 0.001)
                {       steady = false;         }

                }
        }


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