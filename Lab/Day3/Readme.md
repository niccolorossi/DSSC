# Exercise 3

Compute the approximation of PI using the midpoint method (with a REALLY large number of rectangles). 
Reduce the final result in the last process (size – 1) and print the final output from 0.
Compare timing with the OpenMP version, scaling the MPI version up to 2 nodes of Ulysses (excluding operation needed for I/O). 
Reccomendation: use 101 as MPI_TAG.

## Overview

The program ex3.c contains an MPI implementation of the midpoint method for computing Pi. 
It was tested via script.sh, which loads the module impi-trial/5.0.1.035, compiles the code with mpicc -O3
and then executes the resulting file with different numbers of processes.

## In this folder

The file scalability.txt collects the outputs obtained for the various executions of this code:
for each of them it reports the number of processes, the elapsed time and, one line below, the Pi estimate obtained.
The file plot.dat contains time vs. num. of processes for each execution of the program.
time_vs_npes.png is the scalability graph obtained plotting the points of the previous file with gnuplot.

## Results

We can observe that the program scales really well (this doesn't come as a surprise given the embarrassingly
parallel nature of our code). That being said, the execution times are bigger than their OpenMP counterparts
from Exercise 1: this is probably due to the different compilers that I used (icc in ex. 2, mpicc here), with 
icc clearly producing the fastest executable. 
