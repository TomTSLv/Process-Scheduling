/*
 * process.c
 *
 *  Created on: Oct 4, 2015
 *      Author: TianshuLv
 */

#include "stdio.h"

struct process {
	char *status;
	int arrivalTime;
	int cpuMax;
	int totalCpu;
	int multiply;
	int random;
	int cpuBurst;
	int ioBurst;
	int finishTime;
	int turnaroundTime;
	int ioTime;
	int waitTime;
	int originaltotalCpu;
};

