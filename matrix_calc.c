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

#include "aux_stack.c"

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void Usage(char* prog_name);
void *matrizCalc(void* rank);  /* Thread function */
int i, j, m, n, x, y, count_x = 0, count_y = 0;
int size_matrix;
int **a;
int **b;
int **c;
int **v_coordinates;

int main(int argc, char* argv[]) {
    /*-------------------------------- | Main Function |------------------------------------*/
    long       thread;  /* Use long in case of a 64-bit system */
    pthread_t* thread_handles;

    /* Get number of threads from command line */
    if (argc != 3) Usage(argv[0]);
        thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

    /* Get number of size_matrix */
    size_matrix = atoi(argv[2]);
    printf("%d\n", size_matrix);

    /* Dynamic allocation for matrices */
    // Allocating lines
    a = (int**)malloc(size_matrix * sizeof(int*));
    b = (int**)malloc(size_matrix * sizeof(int*));
    c = (int**)malloc(size_matrix * sizeof(int*));

    // Allocating columns
    for(i = 0; i < size_matrix; i++){
        a[i] = (int*)malloc(size_matrix * sizeof(int));
        b[i] = (int*)malloc(size_matrix * sizeof(int));
        c[i] = (int*)malloc(size_matrix * sizeof(int));
    }

    /* Setting random values (matriz C with -1) */
    for(i = 0; i < size_matrix; i++){
        for (j = 0; j < size_matrix; j++) {
            a[i][j] = i + j;
            b[i][j] = i * j;
            c[i][j] = -1;
            printf("%d\t", c[i][j]);
        }
        printf("linha %d \n", i);
    }

    /* Dynamic allocation for aux arrays */
    // v_coordinates = (int**)malloc(size_matrix * size_matrix * sizeof(int*));
    // for (i = 0; i < size_matrix * size_matrix; i++) {
    //         v_coordinates[i] = (int*)malloc(2 * sizeof(int));
    // }

    /* Random allocation of coordinate pairs in a stack */
    // stack initialization
    node *stack = (node*)malloc(sizeof(node));
    if(!stack){
        printf("Sem memória disponivel\n");
        exit(1);
    } else {
        startStack(stack);
    }

    /* Populating arrays with size_matrix possibilities */
    for (i = 0; i < size_matrix; i++) {
        for (j = 0; j < size_matrix; j++)
            push(stack, i, j);
    }

    exibe(stack);

    shuffle(stack);

    srand(time(NULL));
    // random i and j to create pairs.
    // iteration size_matrix * size_matrix times
    // for (i = 0; i < size_matrix * size_matrix; i++) {
    //         do {
    //             // get values between 0 and size_matrix. This value is the index
    //             // of v_lines array
    //             x = rand() % size_matrix;
    //             if(x){}
    //         } while(/* condition */);
    //
    //         y = rand() % size_matrix; // get values between 0 and size_matrix
    // }

    thread_handles = malloc (thread_count * sizeof(pthread_t));

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
        free(v_coordinates);
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
