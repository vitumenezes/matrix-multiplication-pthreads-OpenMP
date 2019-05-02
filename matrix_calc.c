/* File:
*    pth_hello.c
*
* Purpose:
*    Illustrate basic use of pthreads:  create some threads,
*    each of which prints a message.
*
* Input:
*    none
* Output:
*    message from each thread
*
* Compile:  gcc -g -Wall -o pth_hello pth_hello.c -lpthread
* Usage:    ./pth_hello <thread_count>
*
* IPP:   Section 4.2 (p. 153 and ff.)
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void Usage(char* prog_name);
void *matrizCalc(void* rank);  /* Thread function */
int i, j, m, n;
int size_matrix;
int **a;
int **b;
int **c;
int *v_lines;
int *v_columns;

int main(int argc, char* argv[]) {
    /*-------------------------------- | Main Function |------------------------------------*/
    long       thread;  /* Use long in case of a 64-bit system */
    pthread_t* thread_handles;

    /* Get number of threads from command line */
    if (argc != 2) Usage(argv[0]);
        thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

    /* Get number of size_matrix */
    size_matrix = 3; //argv[2];

    /* Dynamic allocation for matrices */
    // Allocating lines
    a = (int**)malloc(size_matrix * sizeof(int));
    b = (int**)malloc(size_matrix * sizeof(int));
    c = (int**)malloc(size_matrix * sizeof(int));

    // Allocating columns
    for(i = 0; i < size_matrix; i++){
        a[i] = (int*)malloc(size_matrix * sizeof(int));
        b[i] = (int*)malloc(size_matrix * sizeof(int));
        c[i] = (int*)malloc(size_matrix * sizeof(int));
    }

    /* Allocation for aux vectors */
    v_lines = (int*)malloc(size_matrix * sizeof(int));
    v_columns = (int*)malloc(size_matrix * sizeof(int));

    /* Setting random values (matriz C with -1) */
    for(i = 0; i < size_matrix; i++){
        for (j = 0; j < size_matrix; j++) {
            a[i][j] = i + j;
            b[i][j] = i * j;
            c[i][j] = -1;
        }
    }

    /* Setting random values (matriz C with -1) */
    for(i = 0; i < size_matrix; i++){
        v_lines[i] = i;
        v_columns[i] = i;
    }

    /* Setting 0 to vector*/
    for (i = 0; i < size_matrix; i++) {
        v_lines[i] = 0;
        v_columns[i] = 0;
    }

    thread_handles = malloc (thread_count*sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)  //Creates thread 0 to thread_count-1
        pthread_create(&thread_handles[thread], NULL,
            matrizCalc, (void*) thread);

        // printf("Hello from the main thread\tNumber of lines: %d\tNumber of columns: %d\n", lines, columns); //this is another thread that has no number. It is know as the main thread.

        for (thread = 0; thread < thread_count; thread++)
            pthread_join(thread_handles[thread], NULL);

        free(thread_handles);
        return 0;

        free(a);
        free(b);
        free(c);
        free(v_columns);
        free(v_lines);
    }  /* main */

    /*------------------------------ | Thread Function | -------------------------------------*/
    void *matrizCalc(void* rank) {
        long my_rank = (long) rank;  /* Use long in case of 64-bit system */
        bool result;
        srand(time(NULL));

        // do {
        //     for (i = 1; result && i < size_matrix; i++){
        //         result &= v_lines[i];
        //     }
        // } while(!result);

        printf("Hello from thread %ld of %d\n", my_rank, thread_count);

        return NULL;
    }  /* Hello */

    /*-------------------------------------------------------------------*/
    void Usage(char* prog_name) {
        fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
        fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
        exit(0);
    }  /* Usage */
