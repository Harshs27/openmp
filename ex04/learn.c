// learning and verifying concepts in this file .. NOT FOR EVALUATION
#include<stdio.h>
#include<omp.h>

int main(int argc, char *argv[]){
	int i, a=1000;
	#pragma omp parallel for firstprivate(a) lastprivate(a)
	for (i=0;i<10;i++){
		a = a+i;
	}
	printf("Value of a: %d\n", a);
	return 0;
}
