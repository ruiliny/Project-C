/***************************************************************************
 *
 *   File        : tasks.c
 *   Student Id  : <INSERT STUDENT ID HERE>
 *   Name        : <INSERT STUDENT NAME HERE>
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tasks.h"
#include <stdbool.h>
#include<float.h>
#define X_LEFT 10
#define X_RIGHT 70
#define Y_DOWN -20
#define Y_UP   20
#define R 24

typedef struct data {
	float x;
	float y;
	float u;
	float v;
}flow_data;


//haha
bool read_data(FILE * fp, flow_data *point)
{
	int count = fscanf(fp, "%f,%f,%f,%f", &(point->x), &(point->y), &(point->u), &(point->v));
	return count == 4;
}

void write_data(FILE * fp, flow_data *point)
{
	fprintf(fp, "%f,%f,%f,%f\n", point->x, point->y, point->u, point->v);
}




void maxveldiff(const char* flow_file)
{
	flow_data max_u;
	flow_data min_u;
	flow_data max_v;
	flow_data min_v;

	//max_u.u = FLT_MIN;
	//min_u.u = FLT_MAX;
	//max_v.v = FLT_MIN;
	//min_v.v = FLT_MAX;

	FILE *fp;
	if ((fp = fopen("flow_data.csv", "r")) == NULL)
	{
		printf("read error\n");
		exit(0);
	}


	char buffer[8];//can't use char buffer * why?
	fgets(buffer, 8, fp);

	read_data(fp, &max_u);
	read_data(fp, &min_u);
	read_data(fp, &max_v);
	read_data(fp, &min_v);

	while (!feof(fp))
	{
		flow_data current;
		read_data(fp, &current);
		if (!read_data(fp, &current)) {
			break;
		}

		if (current.x < 10)

			if (current.x <= 20.0)
				continue;
		if (current.u > max_u.u)
			max_u = current;
		if (current.u < min_u.u)
			min_u = current;
		if (current.v > max_v.v)
			max_v = current;
		if (current.v < min_v.v)
			min_v = current;

	}
	fclose(fp);
	FILE *out = fopen("task1.csv", "w");
	fprintf(fp, "x,y,u,v\n");
	write_data(fp, &max_u);
	write_data(fp, &min_u);
	write_data(fp, &max_v);
	write_data(fp, &min_u);
	fclose(out);
}


typedef struct grid
{
	float x_left;
	float x_right;
	float y_down;
	float y_up;
	float average_x;
	float average_y;
	float average_u;
	float average_v;
	int count;
	float score;
}grid;


void create_grid(grid cells[])
{
	float xd = 1.0*(X_RIGHT - X_LEFT) / R;
	float yd = 1.0*(Y_UP - Y_DOWN) / R;
	for (int i = 0; i < R*R; i++)
	{
		cells[i].x_left = X_LEFT + i % R*xd;
		cells[i].x_right = X_LEFT + (i % R + 1)*xd;
		cells[i].y_down = Y_DOWN + i / R * yd;
		cells[i].y_up = Y_DOWN + (i / R + 1)* yd;
	}
}
int get_index(float x, float y)
{
	float xd = 1.0*(X_RIGHT - X_LEFT) / R;
	float yd = 1.0*(Y_UP - Y_DOWN) / R;
	int row = (int)((x - X_LEFT) / xd);
	int column = (int)((y - Y_DOWN) / yd);
	int index = column * R + row;
	return index;
}

float get_score(float x, float y, float u, float v)
{
	float score = 100 * sqrt(u*u + v * v) / sqrt(x*x + y * y);
	return score;
}

void output_score(FILE * fp, grid *point)
{
	fprintf(fp, "%0.6f,%0.6f,%0.6f,%0.6f,%0.6f\n", point->average_x, point->average_y, point->average_u, point->average_v, point->score);
}


void coarsegrid(const char* flow_file, int resolution)
{
	grid cell[R*R];
	FILE *fp;
	create_grid(cell);
	if ((fp = fopen("flow_data.csv", "r")) == NULL)
	{
		printf("read error\n");
		exit(0);
	}
	char buffer[8];//can't use char buffer * why?
	fgets(buffer, 8, fp);
	for (int i = 0; i < R*R; i++)
	{
		cell[i].average_x = 0;
		cell[i].average_y = 0;
		cell[i].average_u = 0;
		cell[i].average_v = 0;
		cell[i].count = 0;
		cell[i].score = 0;
	}
	//int max = 0;
	//float x, y,u,v = 0;

	while (!feof(fp))
	{
		flow_data current;
		float xd = 1.0*(X_RIGHT - X_LEFT) / R;
		float yd = 1.0*(Y_UP - Y_DOWN) / R;

		if (!read_data(fp, &current))
			break;

		//bool outside_thresh = (current.x <= X_LEFT) || (current.x >= X_RIGHT)
			//|| (current.y <= Y_DOWN) || (current.y >= Y_UP);
		bool within_thresh = (current.x < 70) && (current.x > 10) && (current.y < 20) && (current.y > -20);
		/*bool outside_thresh = (current.x < X_LEFT)||(current.x-X_LEFT) <=FLT_EPSILON
			|| (current.x > X_RIGHT)|| (current.x - X_RIGHT) >= FLT_EPSILON
			|| (current.y < Y_DOWN) || (current.y - Y_DOWN) <= FLT_EPSILON
			|| (current.y > Y_UP) || (current.y - Y_UP) >= FLT_EPSILON;*/

			//if (outside_thresh)
				//continue;
		if (within_thresh)
		{
			int i = get_index(current.x, current.y);

			if ((current.x < cell[i].x_right || fabs(current.x - cell[i].x_right) <= FLT_EPSILON)
				&& (current.x > cell[i].x_left || fabs(current.x - cell[i].x_left) >= FLT_EPSILON)
				&& (current.y < cell[i].y_up || fabs(current.y - cell[i].y_up) <= FLT_EPSILON)
				&& (current.y > cell[i].y_down || fabs(current.y - cell[i].y_down) >= FLT_EPSILON))

			{
				cell[i].count += 1;
				cell[i].average_x = ((cell[i].average_x)*(cell[i].count - 1) + current.x) / cell[i].count;
				cell[i].average_y = ((cell[i].average_y)*(cell[i].count - 1) + current.y) / cell[i].count;
				cell[i].average_u = ((cell[i].average_u)*(cell[i].count - 1) + current.u) / cell[i].count;
				cell[i].average_v = ((cell[i].average_v)*(cell[i].count - 1) + current.v) / cell[i].count;
				cell[i].score = get_score(cell[i].average_x, cell[i].average_y, cell[i].average_u, cell[i].average_v);
			}
			/*if (i > max)
			{
				max = i;
				x = current.x;
				y = current.y;
				u = current.u;
				v = current.v;
			}*/
		}
	}
	//printf("%f %f %f %f %d\n",x,y,u,v, max);
	//bool test = (x < 70) && (x > 10) && (y < 20) && (y > -20);
	//if (test) printf("true");
	int m, n, k;
	grid temp;
	for (m = 0; m < R*R - 1; m++)
	{
		k = m;
		for (n = m + 1; n < R*R; n++)
			if (cell[n].score > cell[k].score)
				k = n;
		temp = cell[k]; cell[k] = cell[m]; cell[m] = temp;
	}
	fclose(fp);
	FILE *out = fopen("task2.csv", "w");
	fprintf(fp, "x,y,u,v,S\n");
	for (int i = 0; i < R*R; i++)
	{
		output_score(fp, &(cell[i]));
	}
	fclose(fp);
}

/*void velstat(const char* flow_file)
{
	printf("velstat() - IMPLEMENT ME!\n");
	exit(EXIT_FAILURE);
}

/*void wakevis(const char* flow_file)
{
	printf("wakevis() Part 1 - IMPLEMENT ME!\n");
	int i,j;
	int n = 12; // Location in x for wake visualization
	float* yheight;
	yheight = (float*) calloc(n,sizeof(float));
	/* Task 4: Part 2, nothing is to be changed here
	   Remember to output the spacing into the array yheight
	   for this to work. You also need to initialize i,j and
	   yheight so the skeleton as it stands will not compile */

	   /* FILE *ft42;
		ft42 = fopen("task4_2.txt","w");
		for (j = 11; j>=0; j--){
		for (i=0;i<yheight[j]-yheight[0]+4;i++){
			fprintf(ft42, " ");
		}
			fprintf(ft42, "*\n");
		}
		for (i=0;i<5; i++){
			fprintf(ft42, "III\n");
		}
		for(j = 0; j<12; j++ ){
			for (i=0;i<yheight[j]-yheight[0]+4;i++){
				fprintf(ft42, " ");
			}
			fprintf(ft42, "*\n");
		}
		fclose(ft42);

		/* Cleanup */
		/*
			free(yheight);

			exit(EXIT_FAILURE);
		}*/
