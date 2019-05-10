#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include "semaphore.h"

#include "stack.c"

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;
pthread_mutex_t mutex;
pthread_mutex_t mutex_threads_working;
void Usage(char* prog_name);
void *matrizCalc(void* rank);  /* Thread function */
int i, j, m, n, count_x = 0, count_y = 0;
int size_matrix, threads_working;
int **a;
int **b;
int **c;
int **d;
int **v_coordinates;
node *stack; // Creating the stack to store the pairs
sem_t *v_semaphores; // Array of semaphores

int main(int argc, char* argv[]) {
    /*--------------------------| Main Function |----------------------------*/

    double total_time;
    clock_t start, end;

    long       thread;
    pthread_t* thread_handles;

    /* Get number of threads from command line */
    if (argc != 3) Usage(argv[0]);
        thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);


    /* Get number of size_matrix */
    size_matrix = atoi(argv[2]);

    threads_working = thread_count;
    /*------------------------ | Dynamic Allocations |-----------------------*/
    /* Dynamic allocation for matrices */
    a = (int**)malloc(size_matrix * sizeof(int*));
    b = (int**)malloc(size_matrix * sizeof(int*));
    c = (int**)malloc(size_matrix * sizeof(int*));
    d = (int**)malloc(size_matrix * sizeof(int*));
    for(i = 0; i < size_matrix; i++){
        a[i] = (int*)malloc(size_matrix * sizeof(int));
        b[i] = (int*)malloc(size_matrix * sizeof(int));
        c[i] = (int*)malloc(size_matrix * sizeof(int));
        d[i] = (int*)malloc(size_matrix * sizeof(int));
    } /* endfor */

    /* Setting random values (matriz C with -1) */
    for(i = 0; i < size_matrix; i++){
        for (j = 0; j < size_matrix; j++) {
            a[i][j] = i + j;
            b[i][j] = i * j;
            c[i][j] = -1;
            d[i][j] = -2;
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

    start = clock();
    /*-------------------------- | Threads Session |--------------------------*/
    thread_handles = malloc (thread_count * sizeof(pthread_t));

    /* Creates thread 0 to thread_count-1 */
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL,
            matrizCalc, (void*) thread);

    /* main thread requests in matriz C */
    srand(time(NULL));
    int index, value, x, y;
    do {
        index = rand() % (size_matrix * size_matrix);
        sem_getvalue(&v_semaphores[index], &value);

        if (value == 1) {
            y = index % size_matrix;
            x = index/size_matrix;

            if (c[x][y] == -1) {
                printf("Wrong value (-1) on C matrix\n");
                exit(0);
            } else if(d[x][y] == -2) {
                d[x][y] = c[x][y];
            }
        }
    } while(threads_working > 0);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    end = clock();
    /*----------------------- | End threads Session |-------------------------*/
    free(thread_handles);
    free(a);
    free(b);
    free(c);
    free(d);
    free(v_coordinates);
    free(stack);

    total_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    FILE *tempo;

    tempo = fopen("tempo_de_exec.txt", "a");

    fprintf(tempo,"Problem Size = %d ----- Thread number = %d ----- Runtime = %f\n", size_matrix, thread_count, total_time);
    fclose(tempo);

    return 0;
}  /* main */

    /*------------------------------ | Thread Function | -------------------------------------*/
void *matrizCalc(void* rank) {
    int x, y, mult_result = 0, count = 0;
    node *tmp;

    // printf("I'm the thread %ld and i get in the function!\n", my_rank);
    srand(time(NULL));

    // Interruped only when the stack is empty
    while(true){
        pthread_mutex_lock(&mutex); // lock to pick up the stack element
        if (stack->prox != NULL){ // empty stack?
            tmp = pop(stack);
            pthread_mutex_unlock(&mutex);
            if(tmp->x ==  NULL){
                printf("Rapaz deu bronca, X é nulo e Y = %d\n", tmp->y);
                exit(0);
            } else if(tmp->y == NULL){
                printf("Rapaz deu bronca, Y é nulo e X = %d\n", tmp->x);
                exit(0);
            }
            x = tmp->x;
            y = tmp->y;
            printf("Valores de x = %d e y = %d e count é %d:\n", x, y, count);

            // individual calculation to each pair
            for (i = 0; i < size_matrix; i++){
                mult_result += a[x][i] * b[i][y];
            }

            // unlock after have done all the calculation

            // post (+1) at the corresponding semaphore
            sem_post(&v_semaphores[x*size_matrix+y]);

            c[x][y] = mult_result; // attribution to C matrix
            mult_result = 0; // reset to the next iteration
            count++;
        } else {
            // another unlock in case of not entering the IF condition
            pthread_mutex_unlock(&mutex);
            break; // if is empty, break the while loop
        }
    }

    // just for Debug
    pthread_mutex_lock(&mutex_threads_working);
    threads_working--;
    pthread_mutex_unlock(&mutex_threads_working);

    // printf("I'm the thread %ld and i get OUT the function. I've calculate %d positions\n", my_rank, count);

    return NULL;
}  /* matrizCalc */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
    fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
    exit(0);
}  /* Usage */
