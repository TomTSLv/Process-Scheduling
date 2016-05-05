/*
 * queue.c
 *
 *  Created on: Oct 5, 2015
 *      Author: TianshuLv
 */

#include<stdio.h>
#include<stdlib.h>
struct Node {
	int data;
	struct Node* next;
};

struct Node* front = NULL;
struct Node* end = NULL;

// Enqueue an integer
void Enqueue(int x) {
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	temp->data =x;
	temp->next = NULL;
	if(front == NULL && end == NULL){
		front = end = temp;
		return;
	}
	end->next = temp;
	end = temp;
}

// Dequeue an integer.
int Dequeue() {
	struct Node* temp = front;
	if(front == NULL) {
		printf("Queue is Empty\n");
	}
	if(front == end) {
		int data=front->data;
		front = end = NULL;
		return data;
	}
	else {
		int data=front->data;
		front = front->next;
		return data;
	}
	free(temp);
	return -1;
}

int isEmpty(){
	return front==NULL;
}

int contains(int num){
	struct Node *temp=front;
	while(temp!=NULL){
		if (temp->data==num) return 1;
		temp=temp->next;
	}
	return 0;
}
