#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

# define N_PARTICLES 10000
# define TOTAL_STEPS 5000
# define STEP_SIZE 500
# define NUM_THREADS 10 

struct x{// Considering m, n, p as the 3 axis 
    double m;
    double n;
    double p;
};

struct y{
    double m;
    double n;
    double p;
};

double get_rand(int a, int b){//gen random num between (a, b)
    return a+(b-a)*(double)rand() / (double)((unsigned)RAND_MAX + 1);
}

double sq(double a){
    return a*a;
}

double dist(struct x a, struct y b){
    return sqrt(sq(a.m-b.m)+sq(a.n-b.n)+sq(a.p-b.p));
}

int main(){
    srand(time(NULL));// each run will create different set of random numbers
    struct x *ptr_x;
    struct y *ptr_y;
    int t, i;
    double avg_dist, distance_moved;
    double delta_t;
    ptr_x = (struct x *)malloc(N_PARTICLES*sizeof(struct x));
    ptr_y = (struct y *)malloc(N_PARTICLES*sizeof(struct y));

    distance_moved = 0;// sum of distance moved in each step ///NOTE: should be reduced variable of FOR LOOP
    FILE *fptr;
    clock_t tic;
    double cpu_time_used;
    tic = clock();
    fptr = fopen("results.txt", "w");
    for(t=0; t<TOTAL_STEPS; t++){// TOTAL_STEPS should not be parallelised...
        #pragma omp parallel for reduction(+:distance_moved) schedule(runtime) num_threads(NUM_THREADS) 
        for (i=0; i<N_PARTICLES; i++){// PARALLELISED,So basically for each step compute the average distance moved by each particle!
            if (t==0){// initialize the positions
                ptr_x[i].m = get_rand(0, 1); ptr_x[i].n = get_rand(0, 1); ptr_x[i].p = get_rand(0, 1);
            }
            else{
                delta_t = t*1.0/10000;// step size
                // find the new position of x
                ptr_y[i].m = get_rand(-1, 1); ptr_y[i].n = get_rand(-1, 1); ptr_y[i].p = get_rand(-1, 1); //NOTE:Check whether new value of y for each step!
                ptr_x[i].m = ptr_x[i].m + sqrt(2*delta_t) * ptr_y[i].m;
                distance_moved = distance_moved + dist(ptr_x[i], ptr_y[i]);
            }
        }
        if(t%STEP_SIZE == 0){// print avg distance moved
            avg_dist = distance_moved/N_PARTICLES;
            printf("Avg distance moved = %f\n", avg_dist);
            fprintf(fptr, "%f\n", avg_dist);
        }
    }
    tic = clock()-tic;
    cpu_time_used = ((double)tic)/CLOCKS_PER_SEC; // returns the time in secs
    printf("Runtime = %f\n", cpu_time_used);
    fclose(fptr);
    return 0;
}

// NOTE : compile gcc -fopenmp assign2.c -lm 
