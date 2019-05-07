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
int i, j, m, n, count_x = 0, count_y = 0;
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

    /* Allocating aux array to all possible (x,y)pairs */
    v_coordinates = (int**)malloc(size_matrix * size_matrix * sizeof(int*));
    for (i = 0; i < size_matrix * size_matrix; i++) {
        v_coordinates[i] = (int*)malloc(2 * sizeof(int));
    }

    /* Setting random values (matriz C with -1) */
    for(i = 0; i < size_matrix; i++){
        for (j = 0; j < size_matrix; j++) {
            a[i][j] = i + j;
            b[i][j] = i * j;
            c[i][j] = -1;
        }
    }

    /* Populating v_coordinates with size_matrix * size_matrix possible pairs */
    for (i = 0; i < size_matrix * size_matrix; i+=size_matrix) {
        for (j = i; j < i + size_matrix; j++){
            v_coordinates[j][0] = count_x;
            v_coordinates[j][1] = count_y;
            count_y++;
        }
        count_y = 0;
        count_x++;
    }

    /* Shuffling the array of coordinates */
    srand(time(NULL));
    for (i = 0; i < size_matrix * size_matrix; i++) {
        int first = rand() % (size_matrix * size_matrix);
        int second = rand() % (size_matrix * size_matrix);

        int x = v_coordinates[first][0];
        int y = v_coordinates[first][1];

        v_coordinates[first][0] = v_coordinates[second][0];
        v_coordinates[first][1] = v_coordinates[second][1];

        v_coordinates[second][0] = x;
        v_coordinates[second][1] = y;
    }

    printf("\n");
    for (i = 0; i < size_matrix * size_matrix; i++) {
        printf("%d%d\t",v_coordinates[i][0], v_coordinates[i][1]);
    }

    thread_handles = malloc (thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)  //Creates thread 0 to thread_count-1
        pthread_create(&thread_handles[thread], NULL,
            matrizCalc, (void*) thread);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    for (i = 0; i < size_matrix; i++) {
        for (j = 0; j < size_matrix; j++) {
            printf("%d\t", c[i][j]);
        }
        printf("\n");
    }

    free(thread_handles);
    free(a);
    free(b);
    free(c);
    free(v_coordinates);

    return 0;
}  /* main */

    /*------------------------------ | Thread Function | -------------------------------------*/
    void *matrizCalc(void* rank) {
        long my_rank = (long) rank;  /* Use long in case of 64-bit system */
        bool result = false;
        int index, x, y, mult_result = 0;
        srand(time(NULL));

        printf("\n\n");
        do {
            index = rand() % size_matrix * size_matrix;

            if (v_coordinates[index][0] != -1) {
                //aqui tem que ter uma condição verificando se o dado
                //está sendo acessado (exclusão)
                if (true) {
                    x = v_coordinates[index][0];
                    y = v_coordinates[index][1];

                    for (i = 0; i < size_matrix; i++){
                        mult_result += a[x][i] * b[i][y];
                        printf("%d * %d + ", a[x][i], b[i][y]);
                    }
                    printf("= %d\n", mult_result);
                    printf("Calculo do elemento %d %d da matriz C = %d\n", x, y, mult_result);
                    c[x][y] = mult_result;
                }
                mult_result = 0;
                v_coordinates[index][0] = -1;
            }

            for (i = 0; !result && i < size_matrix * size_matrix; i++)
                if(v_coordinates[i][0] != -1) result = false;
            printf("%d\n", result);
        } while(!result);

        // printf("Hello from thread %ld of %d\n", my_rank, thread_count);

        return NULL;
    }  /* Hello */

    /*-------------------------------------------------------------------*/
    void Usage(char* prog_name) {
        fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
        fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
        exit(0);
    }  /* Usage */
