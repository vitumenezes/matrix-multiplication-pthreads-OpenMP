# Matrix Calculation With Parallel Programming Using Pthreads and OpenMP (Shared Memory)

Programs built for the subject "Special Topics in Internet of Things" of the bachelor's degree in information technology - BTI of the Federal University of Rio Grande do Norte - UFRN.

## The problem

The work requires the multiplication between two matrices A and B resulting in a matrix C. By definition, the matrices must be squares, that is, the number of rows is equal to the number of columns. Both the serial implementation and the parallel implementation were made.

## The solutions

### Serial

The serial program is easy to understand, and some comments were made to improve that.

To run:

Compilation: `gcc -g -Wall -o serial_matrix serial_matrix_calc.c`  

Execution: `./serial_matrix SZ_OF_PROBLEM`

`SZ_OF_PROBLEM` is a **int** that represents the number of lines of the matrix.

### Parallel

The parallel implementation was constructed so that each thread created individually calculates a random element of the array C. For this, an auxiliary structure (stack) was created. The elements (pairs x and y) are drawn and stacked so that the threads remove them. This removal occurs in a critical region controlled by a Mutex, in the case of the implementation in pthread.

To run:

Compilation: `gcc -g -Wall -o matrix matrix_calc.c -lpthread`

Execution: `./matrix SZ_OF_THREADS SZ_OF_PROBLEM`

`SZ_OF_PROBLEM` is a **int** that represents the number of lines of the matrix.  
`SZ_OF_THREADS` is a **int** that represents the number of threads used.

## Scripts and outputs

Two scripts were created to atutomate programs. The run-time outputs will ve in the **time_de_exec.txt** and **time_de_exec_serial.txt**.
