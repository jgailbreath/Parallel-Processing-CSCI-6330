# Parallel-Processing-Concepts---CSCI-6330-001
Semester Project-Fall 2022

Team: Jesse Gailbreath, Jansen Long, Ryan Morse, Elijah White, Dilafruz Shamsieva

The mat.cpp file is our first simple example of what we intend to accomplish. Currently it takes no command line arguments. The program opens a file "mat.csv" which it expects to share it's own directory. mat.cpp also assumes that the newline characters have been stripped from the csv as well. The individual entries in the csv are read and assigned as cooresponding entries in 2 identical 19x19 matrices. The program then attempts to preform standard serial matrix multiplication and updates one of the 2 matrices with the product values. 

Future iterations of this program will seek to not only preform the matrix multiplication multiple times so as to reach a "steady state", but will also seek to do so applying parallelization and concurency to the end of reducing the time necessary to preform the calculations. 

An example compile statement using the GCC is as follows:
g++ mat.cpp -o mat
