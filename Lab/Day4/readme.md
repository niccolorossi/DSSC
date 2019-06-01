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
