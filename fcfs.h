/*
 * fcfs.c
 *
 *  Created on: Oct 4, 2015
 *      Author: TianshuLv
 */

#include <stdio.h>
#include "process.h"
#include <stdlib.h>
#include <string.h>
#include "queue.h"

int randomOS(int cpuMax, int random) {
	return 1 + random % cpuMax;
}

void printList(struct process *proList,int num){
	printf("%d ",num);
	for (int j=0;j<num;j++){
		printf("( %d %d %d %d ) ",proList[j].arrivalTime,proList[j].cpuMax,proList[j].originaltotalCpu,proList[j].multiply);
	}
	printf("\n");
}

void sort(struct process *proList, int num) {
	for (int i = 0; i < num; i++) {
		for (int j = 0; i + j < num - 1; j++) {
			if (proList[j].arrivalTime > proList[j + 1].arrivalTime) {
				struct process temp = proList[j];
				proList[j] = proList[j + 1];
				proList[j + 1] = temp;
			}
		}
	}
}

void printStatus(struct process *proList, int cycleNum, int num) {
	char *output=(char *)malloc(100*sizeof(char));
	char *cycle=(char *)malloc(10*sizeof(char));
	sprintf(cycle, "%d", cycleNum);
	strcat(output, "Before cycle    ");
	strcat(output, cycle);
	strcat(output, ":   ");
	for (int i = 0; i < num; i++) {
		strcat(output, proList[i].status);
		if (strcmp(proList[i].status, "running") == 0) {
			sprintf(cycle, "%d", proList[i].cpuBurst);
			strcat(output, "  ");
			strcat(output, cycle);
			strcat(output, "  ");
		} else if (strcmp(proList[i].status, "blocked") == 0) {
			sprintf(cycle, "%d", proList[i].ioBurst);
			strcat(output, "  ");
			strcat(output, cycle);
			strcat(output, "  ");
		} else if (strcmp(proList[i].status, "ready") == 0) {
			strcat(output, "  0  ");
		}
		else{
			strcat(output, "  0  ");
		}
	}
	printf("%s\n", output);
}

void runFCFS(struct process *proList, int num,int runMode) {
	int run = 1;
	printf("The original input was:\t");
	printList(proList,num);
	sort(proList, num);
	printf("The (sorted) input is:\t");
	printList(proList,num);
	printf("\n");
	char *output=(char *)malloc(100*sizeof(char));
	int cycleNum = 0; //calculate the number of cycles run
	int blockNum = 0; //calculate cycles with blocked process
	int runNum = 0; //calculate cycles with running process
	int CPURunning = -1; //show which process runs first
	char *cycle=(char *)malloc(10*sizeof(char));
	sprintf(cycle, "%d", cycleNum);
	strcat(output, "Before cycle    ");
	strcat(output, cycle);
	strcat(output, ":   ");
	for (int i = 0; i < num; i++) {
		strcat(output, proList[i].status);
		strcat(output, "  0  ");
	}
	if (runMode==1 || runMode==2) printf("%s\n", output);
	for (int j = 0; j < num; j++) {
		if (strcmp(proList[j].status, "unstarted") == 0
				&& proList[j].arrivalTime == cycleNum) {
			proList[j].status = "ready";
			proList[j].waitTime+=1;
			Enqueue(j);
		}
	}
	FILE *ranNumFp = fopen("random-numbers.txt", "r");
	int random;
	while (run) {
		int block = 0;
		if (CPURunning == -1) {
			if (!isEmpty()){
				fscanf(ranNumFp, "%d", &random);
				if(runMode==2) printf("Find burst when choosing ready process to run %d\n",random);
				CPURunning = Dequeue();
				proList[CPURunning].status = "running";
				proList[CPURunning].cpuBurst = randomOS(proList[CPURunning].cpuMax,
						random);
				proList[CPURunning].ioBurst = proList[CPURunning].cpuBurst
						* proList[CPURunning].multiply;
			}
		}
		else{
			runNum += 1;
			proList[CPURunning].cpuBurst -= 1;
			proList[CPURunning].totalCpu -= 1;
			int previousRunning;
			previousRunning=CPURunning;
			if (proList[CPURunning].cpuBurst == 0) {
				proList[CPURunning].status = "blocked";
				if (!isEmpty()){
					CPURunning=Dequeue();
					fscanf(ranNumFp, "%d", &random);
					if(runMode==2) printf("Find burst when choosing ready process to run %d\n",random);
					proList[CPURunning].status = "running";
					proList[CPURunning].cpuBurst = randomOS(proList[CPURunning].cpuMax,
							random);
					proList[CPURunning].ioBurst = proList[CPURunning].cpuBurst
							* proList[CPURunning].multiply;
				}
				else{
					CPURunning = -1;
				}
			}
			if (proList[previousRunning].totalCpu == 0) {
				proList[previousRunning].status = "terminated";
				proList[previousRunning].finishTime=cycleNum;
				if (previousRunning==CPURunning){
					if ((!isEmpty())){
						CPURunning=Dequeue();
						fscanf(ranNumFp, "%d", &random);
						if(runMode==2) printf("Find burst when choosing ready process to run %d\n",random);
						proList[CPURunning].status = "running";
						proList[CPURunning].cpuBurst = randomOS(proList[CPURunning].cpuMax,
								random);
						proList[CPURunning].ioBurst = proList[CPURunning].cpuBurst
								* proList[CPURunning].multiply;
					}
					else{
						CPURunning = -1;
					}
				}
			}
		}
		int end = 1;
		for(int j=0;j<num;j++){
			if (strcmp(proList[j].status, "terminated") != 0) end = 0;
		}
		if (end == 1){
			break;
		}
		cycleNum++;
		if (runMode==1 || runMode==2) printStatus(proList, cycleNum, num);
		for (int j = 0; j < num; j++) {
			if (strcmp(proList[j].status, "unstarted") == 0
					&& proList[j].arrivalTime == cycleNum) {
				proList[j].status = "ready";
			}
			if (strcmp(proList[j].status, "ready") == 0) {
				proList[j].waitTime += 1;
			}
			if (strcmp(proList[j].status, "blocked") == 0) {
				proList[j].ioBurst -= 1;
				proList[j].ioTime += 1;
				block = 1;
			}
			if (strcmp(proList[j].status, "unstarted") != 0 && proList[j].ioBurst==0 && proList[j].totalCpu!=0 && strcmp(proList[j].status, "terminated") != 0){
				proList[j].status="ready";
			}
		}
		if (block == 1)
			blockNum += 1;
		for (int j = 0; j < num; j++) {
			if (strcmp(proList[j].status, "ready") == 0 && (!contains(j))) {
				Enqueue(j);
			}
		}
	}
	fclose(ranNumFp);
	printf("The scheduling algorithm used was First Come First Served\n\n");
	for (int j=0;j<num;j++){
		proList[j].turnaroundTime=proList[j].finishTime-proList[j].arrivalTime;
		printf("Process %d:\n\t",j);
		printf("(A,B,C,M)=(%d,%d,%d,%d)\n\t",proList[j].arrivalTime,proList[j].cpuMax,proList[j].originaltotalCpu,proList[j].multiply);
		printf("Finishing time: %d\n\t",proList[j].finishTime);
		printf("Turnaround time: %d\n\t",(proList[j].turnaroundTime));
		printf("I/O time: %d\n\t",proList[j].ioTime);
		printf("Waiting time: %d\n\n",proList[j].waitTime-1);
	}
	int sumTurnaround=0;
	int sumWaiting=0;
	for (int j=0;j<num;j++){
		sumTurnaround+=proList[j].turnaroundTime;
		sumWaiting+=proList[j].waitTime-1;
	}
	printf("Summary Data:\n\t");
	printf("Finishing time: %d\n\t",cycleNum);
	printf("CPU Utilization: %f\n\t",((float)runNum/(float)cycleNum));
	printf("I/O Utilization: %f\n\t",((float)blockNum/(float)cycleNum));
	printf("Throughput: %f processes per hundred cycles\n\t",((float)100/cycleNum*num));
	printf("Average turnaround time: %f\n\t",((float)sumTurnaround/(float)num));
	printf("Average waiting time: %f\n\n",((float)sumWaiting/(float)num));
}
