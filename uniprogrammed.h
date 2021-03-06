/*
 * uniprogrammed.h
 *
 *  Created on: Oct 7, 2015
 *      Author: TianshuLv
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runUniprogrammed(struct process *proList, int num, int runMode) {
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
	int CPURunning = 0; //show which process runs first
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
		if (strcmp(proList[CPURunning].status, "running") != 0) {
			if (cycleNum==0){
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
			if (proList[CPURunning].ioBurst==0 && proList[CPURunning].totalCpu!=0) {
				fscanf(ranNumFp, "%d", &random);
				if(runMode==2) printf("Find burst when choosing ready process to run %d\n",random);
				proList[CPURunning].status="running";
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
			if (proList[CPURunning].cpuBurst == 0) {
				proList[CPURunning].status = "blocked";
			}
			if (proList[CPURunning].totalCpu==0){
				proList[CPURunning].status = "terminated";
				proList[CPURunning].finishTime=cycleNum;
				if(!isEmpty()){
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
	printf("The scheduling algorithm used was Uniprocessing\n\n");
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

