#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<omp.h>

# define N_PARTICLES 10000
# define TOTAL_STEPS 5000
# define STEP_SIZE 500 
# define NUM_THREADS 8 

struct x{// Considering m, n, p as the 3 axis 
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

double dist(struct x a, struct x b){
    return sqrt(sq(a.m-b.m)+sq(a.n-b.n)+sq(a.p-b.p));
}

int main(){
    srand(time(NULL));// each run will create different set of random numbers
    struct x *ptr_x;
    struct x *ptr_ini;
    int t, i;
    double avg_dist, distance_moved;
    double delta_t;
    ptr_x = (struct x *)malloc(N_PARTICLES*sizeof(struct x));
	ptr_ini = (struct x *)malloc(N_PARTICLES*sizeof(struct x));

    distance_moved = 0;// sum of distance moved in each step ///NOTE: should be reduced variable of FOR LOOP
    FILE *fptr;
    clock_t tic;
    double cpu_time_used;
    tic = clock();
    fptr = fopen("results.txt", "w");
    for(t=0; t<TOTAL_STEPS; t++){// TOTAL_STEPS should not be parallelised...
        #pragma omp parallel for reduction(+:distance_moved) schedule(runtime) num_threads(NUM_THREADS) 
        for (i=0; i<N_PARTICLES; i++){// PARALLELISED,So basically for each step compute the average distance moved by each particle!
//			int thread_id = omp_get_thread_num();
//			printf("t = %d\tThread %d says: Hello World\n", t, thread_id);

//			if (thread_id == 0) {
//				int nthreads = omp_get_num_threads();
//				printf("t = %d\tThread %d: number of threads is %d\n",t, thread_id, nthreads);
//			}

            if (t==0){// initialize the positions
                ptr_x[i].m = get_rand(0, 1); ptr_x[i].n = get_rand(0, 1); ptr_x[i].p = get_rand(0, 1);

				//saving initial position of each particle 
			//	double ini_m = ptr_x[i].m; double ini_n = ptr_x[i].n; double ini_p = ptr_x[i].p;
				ptr_ini[i].m = ptr_x[i].m; ptr_ini[i].n = ptr_x[i].n; ptr_ini[i].p = ptr_x[i].p;
//				ptr_ini[i].m = 0; ptr_ini[i].n = 0; ptr_ini[i].p =0; 
            }
            else{
//                delta_t = 0.0001;// step size
                // find the new position of x
                ptr_x[i].m = ptr_x[i].m + sqrt(0.0002) * get_rand(-1, 1);
//				printf("%d\tt_val = %f\n",t, ptr_x[i].m);
				ptr_x[i].n = ptr_x[i].n + sqrt(0.0002) * get_rand(-1, 1);
				ptr_x[i].p = ptr_x[i].p + sqrt(0.0002) * get_rand(-1, 1);
                distance_moved = distance_moved + dist(ptr_x[i], ptr_ini[i]); // calculates the displacement of vector x
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
