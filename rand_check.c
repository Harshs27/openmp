#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define NUM_THREADS 4 

// USING Marsaglia random number generator..
unsigned long
x=123456789,y=362436069,z=521288629,w=88675123,v=886756453;
      /* replace defaults with five random seed values in calling program */
unsigned long xorshift(void)
{
	unsigned long t;
	t=(x^(x>>7)); 
	x=y; y=z; z=w; w=v;
	v=(v^(v<<6))^(t^(t<<13)); 
	return (y+y+1)*v;
}

double get_rand(int a, int b){
	return a+(b-a)*(double)rand()/(double)((unsigned)RAND_MAX+1);
}

int main(){
	int i, j;
	double stall, cpu_time_used;
	unsigned long a;
	printf("new_tech = %d\n",(unsigned)RAND_MAX);
	
	clock_t tic;
	tic = clock();
	double sum = 0;
	#pragma omp parallel for num_threads(NUM_THREADS)//reduction(+:sum) schedule(guided, 10000000) num_threads(NUM_THREADS)
	for (i=0; i<100000000; i++){
//		sum = sum + 1;
		if(i<30){
		//	printf("Random num = %f\n", get_rand(0, 1));
			printf("new_tech = %f\n", (double)xorshift()/(unsigned)(2*RAND_MAX+1));
		}
		a = xorshift();
//		stall = get_rand(0, 1);
//		while(j <100000){
//			stall = sqrt(stall)+ 10000*10000;
//			j++;
//		}
	}
	printf("sum = %f\n", sum);
	tic = clock()-tic;
    cpu_time_used = ((double)tic)/CLOCKS_PER_SEC; // returns the time in secs
    printf("Rand Runtime = %f\n", cpu_time_used);
	return 0;
}
