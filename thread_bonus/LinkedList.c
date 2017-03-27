#include "LinkedList.h"




struct Node* newNode(int value)
{
	struct Node *node = malloc(sizeof(struct Node));
	node->value = value;

	return node;
}


struct LinkedList* newLinkedList()
{
	struct LinkedList* list = malloc(sizeof(struct LinkedList));
	list->head = newNode(0);
	list->tail = (list->head);
	list->size = 0;
	return list;

}

void insert(struct LinkedList *list, int value)
{
	(list->tail)->next = newNode(value);
	list->size++;
	list->tail = list->tail->next;
}

void printList(struct LinkedList *list, const char *sep)
{


	int i = 0;
	struct Node *currentNode = list->head;

	for (i = 0; i < list->size; i++) {
		currentNode = currentNode->next;
		printf("%d%s", currentNode->value, sep);
	}



}