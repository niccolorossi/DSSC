# Assignments 4/5
This folder contains the assignments 4 and 5 of the course.

# Distributed identity matrix
nonb_identity.c initializes a distributed identity matrix of size N and prints it on binary file if N>10, else on stdout.
Printing and communication are overlapped via the use of the MPI_Irecv function; the process printing the chunks of matrix
is the root process (0), to which every other process sends its chunk.
To compile the file (with "mpicc nonb_idendity.c") the module impi-trial/5.0.1.035 is needed.
The program is executed with "mpirun -np {num procs} ./a.out".
If used, the binary file name is "matrix.dat".

identity.c is an old version, one in which the overlapping of printing and communication wasn't implemented yet.

# Ring exchange 
ring.c and nonb_ring.c present respectively a non-overlapping and an overlapping version of a communication pattern in which
every process at each iteration sends a buffer to its "rightmost" and receives one from its "leftmost" (as defined in the program) and updates a sum vector. The sum computation is performed while sending the buffer in the overlapping version.

To compile the files (with "mpicc nonb_ring.c" and "mpicc ring.c") the module impi-trial/5.0.1.035 is needed. 

The programs are executed with "mpirun -np {num procs} ./a.out" and the root process (0) prints the elapsed time during
execution: as expected, for buffers of dimension 10E9 and with four processes the time difference is huge in favour of 
nonb_ring.c
