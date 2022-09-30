// This file is a first practice attempt at building a matrix multiplication program.

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
        ifstream mat_file;
        double init_mat1[19][19], init_mat2[19][19], fin_mat[19][19], num;
        int i, j, k;

        mat_file.open("mat.csv");

        if (mat_file)
        {
                while(mat_file >> num)
                {
                        for (i=0;i<19;i++)
                        {
                                for (j=0;j<19;j++)
                                {       init_mat1[i][j] = num;
                                        init_mat2[i][j] = num;  }
                        }
                }
        }
        else
        {
                printf("'mat.csv' was not found");
                return 0;
        }
 
        for (i=0;i<19;i++)
        {
                for (j=0;j<19;j++)
                {
                        for (k=0;k<19;k++)
                        {       fin_mat[i][j] += init_mat1[i][k]*init_mat2[k][j];       }
                }
        }

        for (i=0;i<19;i++)
        {
                for (j=0;j<19;j++)
                {       printf("%f, ",fin_mat[i][j]);   }
                printf("\n");
        }
}
                                                                                                                                                             1,34          Top