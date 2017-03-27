

#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>

struct Node {
	int value;
	struct Node *next;
};

struct LinkedList {
	struct Node *head;
	struct Node *tail;
	int size;
};

struct Node* newNode(int value);
struct LinkedList* newLinkedList();
void insert(struct LinkedList *list, int value);
void printList(struct LinkedList *list, const char *sep);

#endif /* LIST_H */

