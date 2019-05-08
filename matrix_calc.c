#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "semaphore.h"

#include "stack.c"

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;

pthread_mutex_t mutex;

void Usage(char* prog_name);
void *matrizCalc(void* rank);  /* Thread function */
int i, j, m, n, count_x = 0, count_y = 0;
int size_matrix;
int **a;
int **b;
int **c;
int **v_coordinates;
/* Creating the stack */
node *stack;
sem_t *v_semaphores;

int main(int argc, char* argv[]) {
    /*--------------------------| Main Function |----------------------------*/
    long       thread;
    pthread_t* thread_handles;

    /* Get number of threads from command line */
    if (argc != 3) Usage(argv[0]);
        thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

    /* Get number of size_matrix */
    size_matrix = atoi(argv[2]);

    /*------------------------ | Dynamic Allocations |-----------------------*/
    /* Dynamic allocation for matrices */
    a = (int**)malloc(size_matrix * sizeof(int*));
    b = (int**)malloc(size_matrix * sizeof(int*));
    c = (int**)malloc(size_matrix * sizeof(int*));
    for(i = 0; i < size_matrix; i++){
        a[i] = (int*)malloc(size_matrix * sizeof(int));
        b[i] = (int*)malloc(size_matrix * sizeof(int));
        c[i] = (int*)malloc(size_matrix * sizeof(int));
    } /* endfor */

    /* Setting random values (matriz C with -1) */
    for(i = 0; i < size_matrix; i++){
        for (j = 0; j < size_matrix; j++) {
            a[i][j] = i + j;
            b[i][j] = i * j;
            c[i][j] = -1;
        }
    } /* endfor */

    /* Dynamic allocation to stack*/
    stack = (node *)malloc(sizeof(node));
    // Allocating auxiliar array to all possible (x,y)pairs
    v_coordinates = (int**)malloc(size_matrix * size_matrix * sizeof(int*));
    for (i = 0; i < size_matrix * size_matrix; i++) {
        v_coordinates[i] = (int*)malloc(2 * sizeof(int));
    } /* endfor */

    /* Dynamic allocation and fill to array of semaphores */
    v_semaphores = (sem_t *)malloc(size_matrix * size_matrix * sizeof(sem_t));
    for (i = 0; i < size_matrix * size_matrix; i++) {
        sem_init(&v_semaphores[i], 0, 0);
    }

    /* Populating v_coordinates with size_matrix * size_matrix possible pairs
    (in order) */
    for (i = 0; i < size_matrix * size_matrix; i+=size_matrix) {
        for (j = i; j < i + size_matrix; j++){
            v_coordinates[j][0] = count_x;
            v_coordinates[j][1] = count_y;
            count_y++;
        }
        count_y = 0;
        count_x++;
    } /* endfor */

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
    } /* endfor */

    if(!stack){
        printf("Error! No memory available.\n");
        exit(1);
    } else
        startStack(stack);

    /* Pushing all shuffled possibilities to the stack */
    for (i = 0; i < size_matrix * size_matrix; i++) {
        int x = v_coordinates[i][0];
        int y = v_coordinates[i][1];
        push(stack, x, y);
    } /* endfor */

    show(stack);

    thread_handles = malloc (thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)  //Creates thread 0 to thread_count-1
        pthread_create(&thread_handles[thread], NULL,
            matrizCalc, (void*) thread);

    srand(time(NULL));
    do {
        int complete = 0, index, value, x, y;

        index = rand() % (size_matrix * size_matrix);
        sem_getvalue(&v_semaphores[i], &value);

        if (value == 1) {
            // x =  / 
        }

    } while(complete < (size_matrix * size_matrix));


    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    show(stack);

    free(thread_handles);
    free(a);
    free(b);
    free(c);
    free(v_coordinates);
    free(stack);

    return 0;
}  /* main */

    /*------------------------------ | Thread Function | -------------------------------------*/
void *matrizCalc(void* rank) {
    long my_rank = (long) rank;
    int x, y, mult_result = 0;
    node *tmp;

    srand(time(NULL));

    // Interruped only when the stack is empty
    while(true){
        pthread_mutex_lock(&mutex); // lock to pick up the stack element
        if (stack->prox != NULL){ // empty stack?
            tmp = pop(stack);
            x = tmp->x;
            y = tmp->y;

            // individual calculation to each pair
            for (i = 0; i < size_matrix; i++){
                mult_result += a[x][i] * b[i][y];
            }

            // unlock after have done all the calculation
            pthread_mutex_unlock(&mutex);

            // post (+1) at the corresponding semaphore
            sem_post(&v_semaphores[x*size_matrix+y]);

            c[x][y] = mult_result; // attribution to C matrix
            mult_result = 0; // reset to the next iteration
        } else {
            // another unlock in case of not entering the IF condition
            pthread_mutex_unlock(&mutex);
            break; // if is empty, break the while loop
        }
    }

    // just for Debug
    printf("Goodbye from Thread %ld :D\n", my_rank);

    return NULL;
}  /* matrizCalc */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
    fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
    exit(0);
}  /* Usage */
