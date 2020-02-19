/***************************************************************************
 *
 *   File        : main.c
 *   Student Id  : <854255>
 *   Name        : <Ce ZHANG>
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "tasks.h"

char* out_file_name1 = "task1.csv";
char* out_file_name2 = "task2.csv";
char* out_file_name3 = "task3.csv";
char* out_file_name4 = "task4.csv";


int main(int argc, char *argv[]) {
	
	char* file_name = argv[1];
	/* TODO: Parse Command Line Arguments */
	
	
    
	/* Task 1: Find the maximum velocity difference */
	FILE* fp = fopen(file_name, "r");
	FILE* fp1 = safe_fopen(out_file_name1, "w");
	FILE* fp2 = safe_fopen(out_file_name2, "w");
	FILE* fp3 = safe_fopen(out_file_name3, "w");
	FILE* fp4 = safe_fopen(out_file_name4, "w");

	int line_num = lines(fp);//340230
	rewind(fp);

	//struct timeval start;
	//struct timeval stop;

	//gettimeofday(&start, NULL);

	initialization(fp, fp1, line_num);

	/*gettimeofday(&stop, NULL);
	double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 1:  %.2f milliseconds\n", elapsed_ms);
	*/
	/* Task 2: Coarser Grid */
	int k = atoi(argv[2]);
	
	

	/*gettimeofday(&start, NULL);
	coarsegrid(fp2, line_num, k);
	gettimeofday(&stop, NULL);
    elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
    elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
    printf("TASK 2:  %.2f milliseconds\n", elapsed_ms);
	*/
	/* Task 3: Searching */

	/*gettimeofday(&start, NULL);
	searching(fp3, line_num);
	gettimeofday(&stop, NULL);
    elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
    elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
   	printf("TASK 3:  %.2f milliseconds\n", elapsed_ms);
   */ 
	/* Task 4: Statistics */




	//gettimeofday(&start, NULL);
	vortcalc(fp4, line_num);
	/*gettimeofday(&stop, NULL);
	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 4:  %.2f milliseconds\n", elapsed_ms);
	*/

	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	out_file_name1 = NULL;
	out_file_name2 = NULL;
	out_file_name3 = NULL;
	out_file_name4 = NULL;
	file_name = NULL;

	
	
	return (EXIT_SUCCESS);
}


	return (EXIT_SUCCESS);
}
