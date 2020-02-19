/***************************************************************************
 *
 *   File        : tasks.h
 *   Student Id  : <854255>
 *   Name        : <Ce ZHANG>
 *
 ***************************************************************************/
//test
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

//struct timeval start;
//struct timeval stop;

typedef struct
{
	double *x;
	double *y;
	double *u;
	double *v;
}t1_t;

t1_t t1;

typedef struct
{
	double Xav;
	double Yav;
	double Uav;
	double Vav;
	int count;
	double S;
}avg_t;

typedef struct
{
	double *U;

}t3_t;

t3_t t3;

typedef struct
{
	double **X;
	double **Y;
	double **U;
	double **V;
	double **W;
}t4_t;

t4_t t4;

typedef struct node node_t;

struct node {
	void* data;
	node_t* left;
	node_t* right;
};

/* bst type */
typedef struct {
	int num_elements;
	node_t* root;
	void(*del)(void*);
	int(*cmp)(const void*, const void*);
} bst_t;

/* node type */
typedef struct nodeL nodeLinked_t;

struct nodeL {
	void* data;
	nodeLinked_t* next;
};

/* linked list type */
typedef struct {
	int num_elements;
	nodeLinked_t* head;
	nodeLinked_t* tail;
	void(*del)(void*);
} list_t;


#ifndef TASKS_H

void list_process_all(nodeLinked_t *p, double SU, FILE* fp3);

void* list_pop_front(list_t* list);

void list_push_front(list_t* list, void* d);

void list_push_back(list_t* list, void* d);

void list_insert_after(list_t* list, nodeLinked_t* n, void* d);

void* list_pop_back(list_t* list);

list_t* list_new(void(*delfunc)(void*));

void list_free(list_t* list);

bst_t* bst_new(void(*delfunc)(void*), int(*cmpfunc)(const void*, const void*));

void bst_free_subtree(bst_t* bst, node_t* n);

void bst_free(bst_t* bst);

int bst_insert(bst_t* bst, void* d);

void perfect_insert(bst_t* bst, double* array, int low, int high);

void no_free(void* v);

int make_unique(double* array, int n);

void bst_process_all(node_t* n, double VU, FILE* fp3);

void binary_search_recursive(FILE* fp3, double* A, int lo, int hi, double x);

int file_exists(const char* path);

void* safe_malloc(size_t num_bytes);

FILE* safe_fopen(const char* path, const char* mode);

size_t file_size_in_bytes(FILE* fp);

int lines(FILE *fp);

void initialization(FILE* fp, FILE* fp1, int line_num);

void diff(double *maxA, double *minA, double *p, int line_num);

int cmp(const void *a, const void *b);

int doublecmp(const void *a, const void *b);

void maxveldiff(FILE* fp1, int line_num);

void coarsegrid(FILE* fp2, int line_num, int k);

void searching(FILE* fp3, int line_num);

void vortcalc(FILE* fp4, int line_num);

#endif
