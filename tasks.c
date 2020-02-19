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
//#include <sys/time.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>
#include "tasks.h"

//second revise 
#define GRID_X_LEFT 10
#define GRID_X_RIGHT 70
#define GRID_Y_DOWN (-20)
#define GRID_Y_UP 20

typedef struct square {
	float   x_left;  // left barrier
	float   x_right;  // right barrier
	float   y_down;  // bottom barrier
	float   y_up;  // top barrier
	point_t accu;
	int     n;
	float   score;
} square_t;

typedef struct grid {
	square_t *squares;  // 1d array, reso * reso
	int      resolution;
	float    dx;
	float    dy;
} grid_t;

/**
 * from L2/file_io.c
 */
void *safe_malloc(size_t num_bytes) {
	void *ptr = malloc(num_bytes);
	if (ptr == NULL) {
		printf("ERROR: malloc(%lu)\n", num_bytes);
		exit(EXIT_FAILURE);
	}
	return ptr;
}

/**
 * from L2/file_io.c
 */
FILE *safe_fopen(const char *path, const char *mode) {
	FILE *fp = fopen(path, mode);
	if (fp == NULL) {
		perror("file open error.");
		exit(EXIT_FAILURE);
	}
	return fp;
}

bool read_data(FILE *fp, point_t *point) {
	int count = fscanf(fp, "%f,%f,%f,%f",
		&(point->x),
		&(point->y),
		&(point->u),
		&(point->v));
	// it returns the number of values that are successfully read
	// in this case , it is 4
	return count == 4;
}

void write_data(FILE *fp, point_t *point) {
	fprintf(fp, "%f,%f,%f,%f\n", point->x, point->y, point->u, point->v);
}

void add_point(point_t *p1, point_t *p2) {
	p1->x += p2->x;
	p1->y += p2->y;
	p1->u += p2->u;
	p1->v += p2->v;
}

void maxveldiff(const char *flow_file) {
	point_t max_u_p;  // max u point
	point_t min_u_p;  // min u point
	point_t max_v_p;  // max v point
	point_t min_v_p;  // min v point

	max_u_p.u = FLT_MIN;
	min_u_p.u = FLT_MAX;
	max_v_p.v = FLT_MIN;
	min_v_p.v = FLT_MAX;

	char line[MAX_BUF_LEN];
	FILE *fp = safe_fopen(flow_file, "r");
	fgets(line, MAX_BUF_LEN, fp);  // the header, x,y,u,v
	while (!feof(fp)) {
		point_t curr_p;  // current point
		if (!read_data(fp, &curr_p)) {
			break;
		}
		if (curr_p.x <= 20.0) {
			continue;
		}
		if (curr_p.u > max_u_p.u) {
			max_u_p = curr_p;
		}
		if (curr_p.u < min_u_p.u) {
			min_u_p = curr_p;
		}
		if (curr_p.v > max_v_p.v) {
			max_v_p = curr_p;
		}
		if (curr_p.v < min_v_p.v) {
			min_v_p = curr_p;
		}
	}
	fclose(fp);

	fp = safe_fopen("task1.csv", "w");
	fprintf(fp, "x,y,u,v\n");
	write_data(fp, &max_u_p);
	write_data(fp, &min_u_p);
	write_data(fp, &max_v_p);
	write_data(fp, &min_v_p);
	fclose(fp);
}

void grid_init(grid_t *grid, int resolution) {
	grid->dx = 1.0 * (GRID_X_RIGHT - GRID_X_LEFT) / resolution;
	grid->dy = 1.0 * (GRID_Y_UP - GRID_Y_DOWN) / resolution;
	grid->resolution = resolution;

	grid->squares = safe_malloc(sizeof(*grid->squares) * grid->resolution * grid->resolution);

	float y = GRID_Y_DOWN;

	for (int r = 0; r < grid->resolution; ++r) {
		float x = GRID_X_LEFT;

		for (int c = 0; c < grid->resolution; ++c) {
			int index = r * grid->resolution + c;

			square_t *square = &grid->squares[index];
			square->x_left = x;
			square->x_right = x + grid->dx;
			square->y_down = y;
			square->y_up = y + grid->dy;
			square->n = 0;
			square->accu.x = 0;
			square->accu.y = 0;
			square->accu.u = 0;
			square->accu.v = 0;
			x += grid->dx;
		}
		y += grid->dy;
	}
}

void grid_destroy(grid_t *grid) {
	free(grid->squares);
	free(grid);
}

bool grid_add_with_check(square_t *square, point_t *point) {
	float x = point->x;
	float y = point->y;
	if ((square->x_left <= x || fabs(square->x_left - x) <= FLT_EPSILON) &&
		(x <= square->x_right || fabs(square->x_right - x) <= FLT_EPSILON) &&
		(square->y_down <= y || fabs(square->y_down - y) <= FLT_EPSILON) &&
		(y <= square->y_up || fabs(square->y_up - y) <= FLT_EPSILON)) {
		square->n += 1;
		add_point(&square->accu, point);
		return true;
	}
	return false;
}

void grid_handle_adding(const char *flow_file, grid_t *grid) {
	char line[MAX_BUF_LEN];
	FILE *fp = safe_fopen(flow_file, "r");
	fgets(line, MAX_BUF_LEN, fp);  // header

	point_t curr_p;
	while (!feof(fp)) {
		if (!read_data(fp, &curr_p)) {
			break;
		}
		int      idx_c = (int)((curr_p.x - GRID_X_LEFT) / grid->dx);
		int      idx_r = (int)((curr_p.y - GRID_Y_DOWN) / grid->dy);
		for (int row = -1; row <= 1; ++row) {
			for (int col = -1; col <= 1; ++col) {
				int new_r = idx_r + row;
				int new_c = idx_c + col;
				if (new_r >= grid->resolution || new_r < 0 || new_c >= grid->resolution || new_c < 0) {
					continue;
				}
				int index = new_r * grid->resolution + new_c;
				grid_add_with_check(&grid->squares[index], &curr_p);
			}
		}
	}
	fclose(fp);
}

void grid_handle_score(grid_t *grid) {
	for (int row = 0; row < grid->resolution; ++row) {
		for (int col = 0; col < grid->resolution; ++col) {
			int      index = row * grid->resolution + col;
			square_t *square = &grid->squares[index];
			if (square->n > 0) {
				float x_avg = square->accu.x / square->n;
				float y_avg = square->accu.y / square->n;
				float u_avg = square->accu.u / square->n;
				float v_avg = square->accu.v / square->n;
				square->score = 100 * sqrt(u_avg * u_avg + v_avg * v_avg) / sqrt(x_avg * x_avg + y_avg * y_avg);
			}
			else {
				square->score = FLT_MIN;
			}
		}
	}
}

int cmp_square(const void *a, const void *b) {
	square_t *pa = (square_t *)a;
	square_t *pb = (square_t *)b;
	float    diff = pb->score - pa->score;
	if (diff > 0) {
		return 1;
	}
	if (diff < 0) {
		return -1;
	}
	return 0;
}

void coarsegrid(const char *flow_file, int resolution) {
	if (resolution < 1) {
		printf("Invalid resolution: %d", resolution);
		exit(EXIT_FAILURE);
	}

	grid_t *grid = safe_malloc(sizeof(*grid));
	grid_init(grid, resolution);
	grid_handle_adding(flow_file, grid);
	grid_handle_score(grid);

	qsort(grid->squares, grid->resolution * grid->resolution, sizeof(*grid->squares), cmp_square);

	FILE *fp = safe_fopen("task2.csv", "w");
	fprintf(fp, "x,y,u,v,S\n");

	for (int i = 0; i < grid->resolution * grid->resolution; ++i) {
		square_t *square = &grid->squares[i];

		float x_avg = square->accu.x / square->n;
		float y_avg = square->accu.y / square->n;
		float u_avg = square->accu.u / square->n;
		float v_avg = square->accu.v / square->n;
		fprintf(fp, "%f,%f,%f,%f,%f\n", x_avg, y_avg, u_avg, v_avg, square->score);
	}
	fclose(fp);

	grid_destroy(grid);
}

void velstat(const char *flow_file) {
	// from 0.5 to 1.1, need 7 thresholds
	int n = 7;

	float *thres = calloc(n, sizeof(*thres));
	int   *nums = calloc(n, sizeof(*nums));
	int   total = 0;

	float thr = 0.5;

	for (int i = 0; i < n; ++i) {
		thres[i] = thr;
		thr += 0.1;
	}

	char line[MAX_BUF_LEN];
	FILE *fp = safe_fopen(flow_file, "r");
	fgets(line, MAX_BUF_LEN, fp);  // header
	while (!feof(fp)) {
		point_t curr_p;
		if (!read_data(fp, &curr_p)) {
			break;
		}
		total += 1;
		int index = curr_p.u * 10;
		index -= 4;
		if (index >= n) {
			// it is out of range
			continue;
		}
		if (index <= 0) {
			nums[0] += 1;
		}
		else {
			nums[index] += 1;
		}
	}
	fclose(fp);

	for (int i = 1; i < n; ++i) {
		nums[i] += nums[i - 1];
	}
	fp = safe_fopen("task3.csv", "w");
	fprintf(fp, "threshold,points,percentage\n");
	for (int i = 0; i < n; ++i) {
		int num = nums[i];
		fprintf(fp, "%f,%d,%f\n", thres[i], num, 100.0 * num / total);
	}
	fclose(fp);
}

void wakvis_part1(const char *flow_file, point_t *pts, int n) {
	for (int i = 0; i < n; ++i) {
		pts[i].u = FLT_MIN;
	}

	char buffer[MAX_BUF_LEN];
	FILE *fp = safe_fopen(flow_file, "r");
	fgets(buffer, MAX_BUF_LEN, fp);  // header

	float   delta = 0.05;
	point_t curr_p;
	int     start = 10;
	int     stop = start + (n - 1) * 5;
	while (!feof(fp)) {
		if (!read_data(fp, &curr_p)) {
			break;
		}
		int number = round(curr_p.x);
		if (number < start || number > stop) {
			continue;
		}
		if (number % 5 != 0) {
			continue;
		}
		int index = (number - start) / 5;
		if (fabs(curr_p.x - number) > delta) {
			continue;
		}
		if (curr_p.u > pts[index].u) {
			pts[index] = curr_p;
		}
	}
	fclose(fp);
}

void wakevis(const char *flow_file) {
	int   n = 12; // Location in x for wake visualization
	float *yheight;
	yheight = (float *)calloc(n, sizeof(float));

	point_t *pts = (point_t *)calloc(n, sizeof(point_t));

	wakvis_part1(flow_file, pts, n);

	FILE *fp = safe_fopen("task4_1.csv", "w");
	fprintf(fp, "x,y_h\n");
	for (int i = 0; i < n; ++i) {
		yheight[i] = ceil(10 * fabs(pts[i].y));
		if (pts[i].u != FLT_MIN) {
			fprintf(fp, "%f,%f\n", pts[i].x, fabs(pts[i].y));
		}
		else {
			fprintf(fp, "No points.");
		}
	}
	fclose(fp);
	free(pts);

	/* Task 4: Part 2, nothing is to be changed here
	   Remember to output the spacing into the array yheight
	   for this to work. You also need to initialize i,j and
	   yheight so the skeleton as it stands will not compile */

	FILE *ft42 = safe_fopen("task4_2.txt", "w");

	for (int j = 11; j >= 0; j--) {
		for (int i = 0; i < yheight[j] - yheight[0] + 4; i++) {
			fprintf(ft42, " ");
		}
		fprintf(ft42, "*\n");
	}
	for (int i = 0; i < 5; i++) {
		fprintf(ft42, "III\n");
	}
	for (int j = 0; j < 12; j++) {
		for (int i = 0; i < yheight[j] - yheight[0] + 4; i++) {
			fprintf(ft42, " ");
		}
		fprintf(ft42, "*\n");
	}
	fclose(ft42);

	/* Cleanup */
	free(yheight);
}
