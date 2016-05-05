/*
 * main.c
 *
 *  Created on: Oct 4, 2015
 *      Author: TianshuLv
 */
#include "stdio.h"
#include "fcfs.h"
#include "uniprogrammed.h"
#include "rr.h"
#include "sjf.h"

int isAlphabet(char *str){
	if ((str[0]>=65 && str[0]<=90) || (str[0]>=97 && str[0]<=122)){
		return 1;
	}
	else
		return 0;
}

int main(int argc, char *argv[]){
	FILE *fp;
	int num;
	fp = fopen(argv[argc-1], "r");
	int runMode;
	if (argc==3){
		if (strcmp(argv[argc-2], "--verbose")==0){
			runMode=1;
		}
		else if (strcmp(argv[argc-2], "--show-random")==0){
			runMode=2;
		}
	}
	else if (argc==2){
		runMode=0;
	}

	struct process *proListFCFS;
	struct process *proListRR;
	struct process *proListUNI;
	struct process *proListSJF;

	while (!feof(fp)) {
		fscanf(fp,"%d",&num);
		proListFCFS = malloc(sizeof(struct process) * num);
		proListRR = malloc(sizeof(struct process) * num);
		proListUNI = malloc(sizeof(struct process) * num);
		proListSJF = malloc(sizeof(struct process) * num);
		char *str=malloc(sizeof(char)*100);
		int configure;
		for (int i=0;i<num;i++){
			for (int j=0;j<4;j++){
				fscanf(fp,"%s",str);
				proListFCFS[i].status="unstarted";
				proListRR[i].status="unstarted";
				proListUNI[i].status="unstarted";
				proListSJF[i].status="unstarted";
				if (j==0) {
					configure=atoi(&str[1]);
					proListFCFS[i].arrivalTime=configure;
					proListRR[i].arrivalTime=configure;
					proListUNI[i].arrivalTime=configure;
					proListSJF[i].arrivalTime=configure;
				}
				else if (j==1){
					configure=atoi(str);
					proListFCFS[i].cpuMax=configure;
					proListRR[i].cpuMax=configure;
					proListUNI[i].cpuMax=configure;
					proListSJF[i].cpuMax=configure;
				}
				else if (j==2){
					configure=atoi(str);
					proListFCFS[i].originaltotalCpu=configure;
					proListRR[i].originaltotalCpu=configure;
					proListUNI[i].originaltotalCpu=configure;
					proListSJF[i].originaltotalCpu=configure;
					proListFCFS[i].totalCpu=configure;
					proListRR[i].totalCpu=configure;
					proListUNI[i].totalCpu=configure;
					proListSJF[i].totalCpu=configure;
				}
				else {
					configure=atoi(&str[0]);
					proListFCFS[i].multiply=configure;
					proListRR[i].multiply=configure;
					proListUNI[i].multiply=configure;
					proListSJF[i].multiply=configure;
				}
			}
			proListFCFS[i].ioTime=0;
			proListRR[i].ioTime=0;
			proListUNI[i].ioTime=0;
			proListSJF[i].ioTime=0;
			proListFCFS[i].waitTime=0;
			proListRR[i].waitTime=0;
			proListUNI[i].waitTime=0;
			proListSJF[i].waitTime=0;
		}
		fscanf(fp,"%s",str);
		if (isAlphabet(str)) break;
	}

	printf("==========================  Start of FCFS  =============================\n");
	runFCFS(proListFCFS,num,runMode);
	printf("==========================  End of FCFS  =============================\n\n");
	printf("==========================  Start of RR  =============================\n");
	runRR(proListRR,num,runMode);
	printf("==========================  End of RR  =============================\n\n");
	printf("==========================  Start of Uniprogramming  =============================\n");
	runUniprogrammed(proListUNI,num,runMode);
	printf("==========================  End of Uniprogramming  =============================\n\n");
	printf("==========================  Start of SJF  =============================\n");
	runSJF(proListSJF,num,runMode);
	printf("==========================  End of SJF  =============================\n\n");

	fclose(fp);

	return 0;
}
