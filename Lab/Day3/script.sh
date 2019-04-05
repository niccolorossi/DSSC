 #!/bin/bash

cd ~/PP/DSSC/Lab/Day3

module load impi-trial/5.0.1.035

mpicc ex3.c

for procs in 1 2 4 8 16 20 40; do
    mpirun -np ${procs} ./a.out >> scalability.txt
done

exit