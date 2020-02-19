/***************************************************************************
 *
 *   File        : tasks.c
 *   Student Id  : <854255>
 *   Name        : <Ce ZHANG>
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "tasks.h"

#define BST_SUCCESS 1
#define BST_FAILURE 0
#define BST_PREORDER 0
#define BST_INORDER 1
#define BST_POSTORDER 2
#define maxX 70
#define minX 10
#define maxY 20
#define minY -20
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


/* process all elements in the list */
void list_process_all(nodeLinked_t *p, double SU, FILE* fp3)
{
	double close = fabs(*(double*)(p->data)-SU);
	while (p) {
		if (fabs(*(double*)(p->data) - SU) <= close) {
			close = fabs(*(double*)(p->data) - SU);
			fprintf(fp3, "%.6lf,", (*(double*)p->data));
		}
		p = p->next;
	}
	fprintf(fp3, "\n");
}

/* remove node at the front of the list */
void* list_pop_front(list_t* list)
{
	assert(list != NULL);
	assert(list->num_elements > 0);
	nodeLinked_t* old;
	assert(list->head != NULL);
	old = list->head;
	list->head = list->head->next;
	void* d = old->data;
	free(old);
	list->num_elements--;
	if (list->num_elements == 0) {
		list->head = NULL;
		list->tail = NULL;
	}
	return d;
}

/* add node add the front of the list */
void list_push_front(list_t* list, void* d)
{
	assert(list != NULL);
	nodeLinked_t* new = (nodeLinked_t*)malloc(sizeof(nodeLinked_t));
	assert(new);
	new->data = d;
	/*printf("%lf\n", (*(double*)d));*/
	new->next = list->head;
	list->head = new;
	if (list->tail == NULL)
		list->tail = new;
	list->num_elements++;
}

/* add node add the back of the list */
void list_push_back(list_t* list, void* d)
{
	assert(list != NULL);
	nodeLinked_t* new = (nodeLinked_t*)malloc(sizeof(nodeLinked_t));
	assert(new != NULL);
	new->data = d;
	new->next = NULL;
	if (list->tail)
		list->tail->next = new;
	list->tail = new;
	if (list->head == NULL)
		list->head = new;
	list->num_elements++;
}

/* TODO: implement a function inserts a new node (for item d)
after node n
*/
void list_insert_after(list_t* list, nodeLinked_t* n, void* d)
{
	assert(list != NULL);
	assert(n != NULL);
	assert(d != NULL);

	nodeLinked_t* new = (nodeLinked_t*)malloc(sizeof(nodeLinked_t));
	assert(new != NULL);
	new->data = d;

	// link the new node
	new->next = n->next;
	n->next = new;

	// make sure the list pointers are up to date
	if (list->tail == n)
		list->tail = new;
	list->num_elements++;
}

/* TODO: implement a function which removes the
last element in the list
*/
void* list_pop_back(list_t* list)
{
	assert(list != NULL);
	assert(list->num_elements > 0);
	return NULL;
}

/* create a new linked list structure */
list_t* list_new(void(*delfunc)(void*))
{
	list_t* list;
	list = (list_t*)malloc(sizeof(list_t));
	assert(list != NULL);
	list->head = NULL;
	list->tail = NULL;
	list->num_elements = 0;
	list->del = delfunc;
	return list;
}

/* free all memory associated with a list */
void list_free(list_t* list)
{
	assert(list != NULL);
	free(list);
}

/* create a new empty bst structure */
bst_t* bst_new(void(*delfunc)(void*), int(*cmpfunc)(const void*, const void*))
{
	bst_t* bst;
	bst = (bst_t*)malloc(sizeof(bst_t));
	assert(bst != NULL);
	bst->root = NULL;
	bst->num_elements = 0;
	bst->del = delfunc;
	bst->cmp = cmpfunc;
	return bst;
}

/* free all memory assocated with a subtree */
void bst_free_subtree(bst_t* bst, node_t* n)
{
	assert(bst != NULL);
	if (n) {
		bst_free_subtree(bst, n->left);
		bst_free_subtree(bst, n->right);
		bst->del(n->data);
		free(n);
		bst->num_elements--;
	}
}

/* free all memory associated with a bst */
void bst_free(bst_t* bst)
{
	assert(bst != NULL);
	bst_free_subtree(bst, bst->root);
	free(bst);
}

/* insert a new element into the bst */
int bst_insert(bst_t* bst, void* d)
{
	assert(bst != NULL);
	assert(d != NULL);
	node_t* parent = NULL;
	node_t* tmp = bst->root;
	while (tmp) {
		parent = tmp;
		if (bst->cmp(tmp->data, d) < 0) { // element is smaller
			tmp = tmp->left;
		}
		else if (bst->cmp(tmp->data, d) > 0) { // element is bigger
			tmp = tmp->right;
		}
		else {
			/* ALREADY EXISTS! -> do nothing and return ERROR */
			return BST_FAILURE;
		}
	}

	/* insert as child of parent */
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	assert(new_node != NULL);
	new_node->data = d;
	new_node->left = NULL;
	new_node->right = NULL;
	if (parent != NULL) {
		if (bst->cmp(parent->data, d) < 0) { // element is smaller
			assert(parent->left == NULL);
			parent->left = new_node;
		}
		else {
			assert(parent->right == NULL);
			parent->right = new_node;
		}
	}
	else {
		assert(bst->root == NULL);
		bst->root = new_node;
	}
	bst->num_elements++;

	return BST_SUCCESS;
}

// Implement this function (0)
void perfect_insert(bst_t* bst, double* array, int low, int high)
{
	if (low <= high) {
		// Choose root from array and insert
		// Recursively do the same on left and right (1)
		int mid = low + (high - low) / 2;
		double* ptr = array + mid;
		bst_insert(bst, ptr);
		perfect_insert(bst, array, low, mid - 1);
		perfect_insert(bst, array, mid + 1, high);
	}
}

void no_free(void* v)
{
}

int make_unique(double* array, int n) // eliminate the number of same values
{
	int dest = 0;
	int itr = 1;
	while (itr != n) {
		if (array[dest] != array[itr]) {
			array[++dest] = array[itr];
		}
		itr++;
	}
	return dest + 1;
}

/*print out the searching value in the balanced BST*/
void bst_process_all(node_t* n, double VU, FILE* fp3)  
{
	if (*(double*)(n->data) != VU)
	{
		/*fprintf(fp3, "%lf,", (*(double*)n->data));*/
		fprintf(fp3, "%.6lf,", (*(double*)n->data));
		if (*(double*)(n->data) < VU)
		{
			bst_process_all(n->left, VU, fp3);
		}
		else
		{
			bst_process_all(n->right, VU, fp3);
		}

	}
	else if (*(double*)(n->data) == VU)
	{
		//fprintf(fp3, "%lf,", (*(double*)n->data));
		fprintf(fp3, "%.6lf,", (*(double*)n->data));
	}
	fprintf(fp3, "\n");
}

/*binary_search_recursive function*/
void binary_search_recursive(FILE* fp3, double* A, int lo, int hi, double x)
{
	if (lo >= hi) {
		exit(EXIT_FAILURE);
	}
	int mid = (lo + hi) / 2;
	double M = A[mid];
	fprintf(fp3, "%.6lf,", M);
	if (x < M) {
		binary_search_recursive(fp3, A, lo, mid, x);
	}
	else if (x > M) {
		binary_search_recursive(fp3, A, mid + 1, hi, x);
	}
}

int file_exists(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (fp == NULL) {
		return 0;
	}
	fclose(fp);
	return 1;
}

void* safe_malloc(size_t num_bytes)
{
	void* ptr = malloc(num_bytes);
	if (ptr == NULL) {
		printf("ERROR: malloc(%lu)\n", num_bytes);
		exit(EXIT_FAILURE);
	}
	return ptr;
}

FILE* safe_fopen(const char* path, const char* mode)
{
	FILE* fp = fopen(path, mode);
	if (fp == NULL) {
		perror("file open error.");
		exit(EXIT_FAILURE);
	}
	return fp;
}

size_t file_size_in_bytes(FILE* fp)
{
	assert(fp != NULL);
	long cur_pos = ftell(fp);
	if (cur_pos == -1) {
		perror("ftell error.");
		exit(EXIT_FAILURE);
	}
	/* seek to the end */
	int ret = fseek(fp, 0L, SEEK_END);
	if (ret == -1) {
		perror("file seek error.");
		exit(EXIT_FAILURE);
	}
	long fs = ftell(fp);

	/* seek back to the previous position */
	ret = fseek(fp, cur_pos, SEEK_SET);
	if (ret == -1) {
		perror("file seek error.");
		exit(EXIT_FAILURE);
	}
	return (size_t)fs;
}

void free2d(double **X, int n)  // free 2D array
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (X[i] != NULL) 
		{
			free(X[i]);
		}
	}
	free(X);
	X = NULL;
}

void free1d(double *X)  // free 1D array
{
	free(X);
	X = NULL;
}

// free all memorys in tasks
void freealltask(int count)
{
	free1d(t1.x);
	free1d(t1.y);
	free1d(t1.u);
	free1d(t1.v);
	
	free2d(t4.U, count);
	free2d(t4.V, count);
	free2d(t4.W, count);
	free2d(t4.X, count);
	free2d(t4.Y, count);
}

// determining the rows in the flow_data file
int lines(FILE *fp) 
{
	int line_num = 0;
	char line;
	while (!feof(fp))
	{
		line = fgetc(fp);
		if (line == '\n') {
			line_num++;
		}
	}
	return line_num;
}

/*initialise the arrays*/
void initialization(FILE* fp, FILE* fp1, int line_num)
{
	
	t1.x = (double*)calloc(line_num, sizeof(double));
	t1.y = (double*)calloc(line_num, sizeof(double));
	t1.u = (double*)calloc(line_num, sizeof(double));
	t1.v = (double*)calloc(line_num, sizeof(double));
	char a, b, c, d;
	double one, two, three, four;
	int i=0;
	fscanf(fp, "%c,%c,%c,%c\n", &a, &b, &c, &d);
	while (fscanf(fp, "%lf,%lf,%lf,%lf\n", &one, &two, &three, &four) == 4) 
	{
		t1.x[i] = one;
		t1.y[i] = two;
		t1.u[i] = three;
		t1.v[i] = four;
		i++;
	}

	maxveldiff(fp1, line_num);
}

/*TASK 1*/
void maxveldiff(FILE* fp1, int line_num)
{
	double max1 = t1.u[0];
	double min1 = t1.u[0];
	double max2 = t1.v[0];
	double min2 = t1.v[0];
	int i = 0;
	int maxU = 0;
	int minU = 0;
	int maxV = 0;
	int minV = 0;

	// compare the values, find the maxium and minium values
	for (i = 0; i < line_num; i++)
	{
		if (t1.x[i] > 20) {
			if (max1 < t1.u[i])
			{
				max1 = t1.u[i];
				maxU = i;
			}
			if (min1 > t1.u[i])
			{
				min1 = t1.u[i];
				minU = i;
			}
			if (max2 < t1.v[i])
			{
				max2 = t1.v[i];
				maxV = i;
			}
			if (min2 > t1.v[i])
			{
				min2 = t1.v[i];
				minV = i;
			}
		}
	}

	fprintf(fp1, "x,y,u,v\n");
	fprintf(fp1, "%.6lf,%.6lf,%.6lf,%.6lf\n", t1.x[maxU], t1.y[maxU], t1.u[maxU], t1.v[maxU]);
	fprintf(fp1, "%.6lf,%.6lf,%.6lf,%.6lf\n", t1.x[minU], t1.y[minU], t1.u[minU], t1.v[minU]);
	fprintf(fp1, "%.6lf,%.6lf,%.6lf,%.6lf\n", t1.x[maxV], t1.y[maxV], t1.u[maxV], t1.v[maxV]);
	fprintf(fp1, "%.6lf,%.6lf,%.6lf,%.6lf\n", t1.x[minV], t1.y[minV], t1.u[minV], t1.v[minV]);

}

/*Find the maxium and minium values*/
void diff(double *maxA, double *minA, double *p, int line_num)
{
	*maxA = p[0];
	*minA = p[0];
	int i = 0;
	for (i = 0; i < line_num-2; i++)
	{
		if (*maxA < p[i])
		{
			*maxA = p[i];
		}
		if (*minA > p[i])
		{
			*minA = p[i];
		}
	}
}

int cmp(const void *a, const void *b)
{
	return (*(avg_t *)a).S> (*(avg_t *)b).S ? 1 : -1;
}

/* Task 2 */
void coarsegrid(FILE* fp2, int line_num, int k)
{
	double deltaX = 0;
	double deltaY = 0;
	/*double *Px, *Py;
	Px = t1.x;
	Py = t1.y;*/
	int i = 0;
	int j = 0;
	int h = 0;
	avg_t* av = (avg_t*)calloc(k*k, sizeof(avg_t));  // calloc a array of struct to store the sum values of x,y,u,v and count 
	/*diff(&maxX, &minX, Px, line_num);
	diff(&maxY, &minY, Py, line_num);*/
	deltaX = (maxX - minX) / (double)k;
	deltaY = (maxY - minY) / (double)k;

	//for each value eat in , calculate the sum once
	while (i < line_num-2)
	{
		for (j = 0; j < k; j++)
		{
			for (h = 0; h < k; h++)
			{
				if ((t1.x[i] >= (minX + j * deltaX)) && (t1.x[i] < (minX + (j + 1)*deltaX)))
				{
					if ((t1.y[i] >= (minY + h * deltaY)) && (t1.y[i] < (minY + (h + 1)*deltaY)))
					{
						av[k*j + h].Xav = av[k*j + h].Xav + t1.x[i];
						av[k*j + h].Yav = av[k*j + h].Yav + t1.y[i];
						av[k*j + h].Uav = av[k*j + h].Uav + t1.u[i];
						av[k*j + h].Vav = av[k*j + h].Vav + t1.v[i];
						av[k*j + h].count++;
					}
				}
				
			}
		}
		i++;
	}

	fprintf(fp2, "x,y,u,v,S\n");
	
	// calculate the average values of x,y,u,v
	for (i = 0; i < k*k; i++)
	{
		if (av[i].count != 0)
		{
			av[i].Xav = av[i].Xav / av[i].count;
			av[i].Yav = av[i].Yav / av[i].count;
			av[i].Uav = av[i].Uav / av[i].count;
			av[i].Vav = av[i].Vav / av[i].count;
			av[i].S = ( 100 * sqrt(pow(av[i].Uav, 2) + pow(av[i].Vav, 2)) / sqrt(pow(av[i].Xav, 2) + pow(av[i].Yav, 2)));
		}
	}

	// qsort
	qsort(av, k*k, sizeof(avg_t), cmp);
	
	int f = k * k - 1;

	// print out
	for (i = f; i >= 0 ; i--)
	{
		fprintf(fp2, "%.6lf,%.6lf,%.6lf,%.6lf,%.6lf\n", av[i].Xav, av[i].Yav, av[i].Uav, av[i].Vav, av[i].S);
	}
	if (av != NULL) 
	{
		free(av);
	}

};

int doublecmp(const void *a, const void *b)
{
	return (*(double *)a)> (*(double *)b) ? 1 : -1;
}

/* Task 3 */
void searching(FILE* fp3, int line_num)
{
	int i = 0;
	int j = 0;
	int count = 0;
	double SU = 0;
	double maxU, minU;
	double *Pu;
	
	// find the number of rows that y == 0
	while (i < line_num - 2) 
	{
		if (t1.y[i] == 0.00) 
		{
			count++;
		}
		i++;
	}
	
	t3.U = (double*)calloc(count, sizeof(double));  // Calloc a struct of array to store the values of U where y = 0
	Pu = t3.U;
	i = 0;
	while (i < line_num - 2) 
	{
		if (t1.y[i] == 0.000) 
		{
			t3.U[j] = t1.u[i];
			
			j++;
		}
		i++;
	}

	diff(&maxU, &minU, Pu, count+2);
	SU = maxU * 0.4;
	qsort(t3.U, count, sizeof(t3.U[0]), doublecmp); // sort array

	gettimeofday(&start, NULL);

	double VU;
	i = 0;
	double close = fabs(t3.U[0]- SU);
	while (i < count) 
	{
		if (fabs(t3.U[i] - SU) < close)  // Finding the closest value
		{
			close = fabs(t3.U[i] - SU);
			fprintf(fp3, "%.6lf,", t3.U[i]);
			VU = t3.U[i];
		}
		i++;
	}
	fprintf(fp3, "\n");
	gettimeofday(&stop, NULL);
	double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 Array Linear Search:  %.2f milliseconds\n", elapsed_ms);

	gettimeofday(&start, NULL);
	binary_search_recursive(fp3, t3.U, 0, count, VU); //binary search tree output
	fprintf(fp3, "\n");
	gettimeofday(&stop, NULL);
	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 Array Binary Search:  %.2f milliseconds\n", elapsed_ms);

	bst_t* bstbalance = bst_new(no_free, doublecmp); // creat balanced BST
	list_t* list = list_new(free); // creat Linked list

	///* insert values into Linked list */
	i = 0;
	assert(list->num_elements == 0);
	while (i < count) 
	{
		list_push_back(list, Pu+i);
		/*printf("%lf\n", *(t3.U + i));*/
		i++;
	}
	assert(list->num_elements == count);

	gettimeofday(&start, NULL);
	list_process_all(list->head, VU, fp3); // Linked list output
	gettimeofday(&stop, NULL);
	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 List Linear Search:  %.2f milliseconds\n", elapsed_ms);

	/****************************************************/
	count = make_unique(t3.U, count); // Balanced BST part

	perfect_insert(bstbalance, t3.U, 0, count-1);

	gettimeofday(&start, NULL);
	bst_process_all(bstbalance->root, VU, fp3); // Balanced BST output
	gettimeofday(&stop, NULL);
	elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
	printf("TASK 3 BST Search:  %.2f milliseconds\n", elapsed_ms);

	list_free(list); // free linked list
	bst_free(bstbalance); // free bst
	if(t3.U != NULL)
	{
		free(t3.U);
	}
	Pu = NULL;
	
}

/* Task 4 */
void vortcalc(FILE* fp4, int line_num)
{
	int i = 0;
	int j = 0;
	int z = 0;
	int h = 0;
	int c = 0;
	int *count = (int*)calloc(line_num, sizeof(int));
	double num1 = 0, den1 = 0, num2 = 0, den2 = 0;
	for (i=1; i < line_num - 2; i++) 
	{
		if (t1.y[i] == t1.y[i - 1]) 
		{
			count[j] = count[j] + 1; // calculate the number of same value of data.
		}
		else
		{
			count[j]++; // each stack of y[i] == y[i - 1] will miss one count.
			j++;  // calculate the number of different block of data.
		}
	}
	count[j]++; // the last stack will miss two count since i is greater than (line_num - 2) the loop is finish.
	count[j]++;
	j++; // just for make easier to calculate, j = 397 right now.


	// Calloc 2D array
	t4.X = (double**)calloc(count[j - 1], sizeof(double)); // the number of different value of y
	for (i = 0; i < count[j - 1]; i++)
	{
		t4.X[i] = (double*)calloc(j, sizeof(double)); // the number of same value of y
	}
	t4.Y = (double**)calloc(count[j - 1], sizeof(double));
	for (i = 0; i < count[j - 1]; i++)
	{
		t4.Y[i] = (double*)calloc(j, sizeof(double));
	}
	t4.U = (double**)calloc(count[j - 1], sizeof(double));
	for (i = 0; i < count[j - 1]; i++)
	{
		t4.U[i] = (double *)calloc(j, sizeof(double));
	}
	t4.V = (double**)calloc(count[j - 1], sizeof(double));
	for (i = 0; i < count[j - 1]; i++)
	{
		t4.V[i] = (double *)calloc(j, sizeof(double));
	}
	t4.W = (double**)calloc(count[j - 1], sizeof(double)); 
	for (i = 0; i < count[j - 1]; i++)
	{
		t4.W[i] = (double *)calloc(j, sizeof(double));
	}

	// j = 397 , count[0] = 857
	// assign value for each 2D array

	i = 0; // reset i
	while (i < line_num - 2)
	{
		for (z = 0; z <= j-1; z++) // 0 - 396
		{
			for (h = 0; h <= count[0] - 1; h++) // 0 - 856
			{
				t4.X[h][z] = t1.x[i];
				t4.Y[h][z] = t1.y[i];
				t4.U[h][z] = t1.u[i];
				t4.V[h][z] = t1.v[i];
				i++;
			}
		}
	}

	// The indices in the loop below for i go from 0 : n − 2 and for j go from 0
	for (z = 0; z <= j - 2; z++) // 0 - 395
	{
		for (h = 0; h <= count[0] - 2; h++) // 0 - 855
		{
			num1 = t4.V[h + 1][z] - t4.V[h][z]; // first numerator in the equation
			den1 = t4.X[h + 1][z] - t4.X[h][z]; // first denominator in  the equation
			num2 = t4.U[h][z + 1] - t4.U[h][z];
			den2 = t4.Y[h][z + 1] - t4.Y[h][z];
			t4.W[h][z] = (num1 / den1) - (num2 / den2);
		}
	}

	// For datapoints with indices n − 1 or m − 1, the value of vorticity will be given by
	int tmp1 = count[j - 1] - 1; // 856 (n-1)
	int tmp2 = j - 1; // 396  (m-1)

	for (z = 0; z < tmp2; z++)  // 0 - 395
	{
		num1 = t4.V[tmp1][z] - t4.V[tmp1 - 1][z];
		den1 = t4.X[tmp1][z] - t4.X[tmp1 - 1][z];
		num2 = t4.U[tmp1][z + 1] - t4.U[tmp1][z];
		den2 = t4.Y[tmp1][z + 1] - t4.Y[tmp1][z];
		t4.W[tmp1][z] = (num1 / den1) - (num2 / den2);
	}

	for (h = 0; h < tmp1; h++) // 0 - 855
	{
		num1 = t4.V[h + 1][tmp2] - t4.V[h][tmp2];
		den1 = t4.X[h + 1][tmp2] - t4.X[h][tmp2];
		num2 = t4.U[h][tmp2] - t4.U[h][tmp2 - 1];
		den2 = t4.Y[h][tmp2] - t4.Y[h][tmp2 - 1];
		t4.W[h][tmp2] = (num1 / den1) - (num2 / den2);
	}

	num1 = t4.V[tmp1][tmp2] - t4.V[tmp1 - 1][tmp2];
	den1 = t4.X[tmp1][tmp2] - t4.X[tmp1 - 1][tmp2];
	num2 = t4.U[tmp1][tmp2] - t4.U[tmp1][tmp2 - 1];
	den2 = t4.Y[tmp1][tmp2] - t4.Y[tmp1][tmp2 - 1];
	t4.W[tmp1][tmp2] = (num1 / den1) - (num2 / den2);

	c = (j)*(count[0]);

	double *w = (double*)calloc(c, sizeof(double));  // Calloc array w,  convert 2D array into 1D，easier to qsort.

	i = 0; // initialize the i

	/* assign all the values in the t4.W array into W array */
	while (i < c)
	{
		for (z = 0; z < j; z++)
		{
			for (h = 0; h < (count[j - 1]); h++)
			{
				w[i] = t4.W[h][z];  // convert 2D array into 1D array, easier to qsort
				i++;
			}
		}
	}

	qsort(w, c, sizeof(w[0]), doublecmp); // qsort

	fprintf(fp4, "omega\n");
	i = c-1;  // inverse print out the value stored in the w array
	while (i >= 0)
	{
		fprintf(fp4, "%.6lf\n", w[i]);
		i--;
	}

	freealltask(count[j-1]);  // free all calloced arrays

	assert(count != NULL);
	free(count);
	assert(w != NULL);
	free(w);
	
	

}


