 #!/bin/bash

cd ~/PP/DSSC/Lab/Day3

module load impi-trial/5.0.1.035

mpicc ex3.c

mpirun -np 20 ./a.out
   
exit