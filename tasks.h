/***************************************************************************
 *
 *   File        : tasks.h
 *   Student Id  : <INSERT STUDENT ID HERE>
 *   Name        : <INSERT STUDENT NAME HERE>
 *
 ***************************************************************************/

#ifndef TASKS_H

#define MAX_BUF_LEN 2048

typedef struct point {
	float x;
	float y;
	float u;
	float v;
} point_t;

void maxveldiff(const char* flow_file);

void coarsegrid(const char* flow_file, int resolution);

void velstat(const char* flow_file);

void wakevis(const char* flow_file);

#endif
#pragma once
