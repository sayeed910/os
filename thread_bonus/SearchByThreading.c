
#include "LinkedList.h"
#include <pthread.h>
#include <time.h>

#define SIZE 1000
#define FILE_NAME "numbers.txt"
#define NUMBER_OF_THREADS 10

struct SearchRange {
	int base;
	int limit;
};

struct ThreadParameter {
	FILE *fileToSearch;
	int valueToSearch;
	struct SearchRange *searchRange;
	struct LinkedList *hits;
};

struct SearchRange* newSearchRange(int base, int limit);
struct ThreadParameter* newThreadParameter(FILE *file,
	int value, struct SearchRange *range, struct LinkedList* hits);
void writeRandomNumbersToFile(FILE *file);
void moveFileCursor(int position, FILE *file);
void* linearSearch(void* args);
void createSearchThreads(int valueToSearch, struct LinkedList **hits);
void joinSearchThreads(struct LinkedList **hits);
void displayResult(struct LinkedList **hits);
int getSearchValue();

pthread_t threadId[NUMBER_OF_THREADS];
int valueFound = 0;

int main(void)
{
	int valueToSearch;
	struct LinkedList * hitIndices[NUMBER_OF_THREADS];

	writeRandomNumbersToFile(fopen(FILE_NAME, "w"));
	valueToSearch = getSearchValue();
	createSearchThreads(valueToSearch, hitIndices);
	displayResult(hitIndices);

	return(EXIT_SUCCESS);
}

void writeRandomNumbersToFile(FILE *numberFile)
{
	
	if (numberFile == NULL) {
		puts("Could not open file.");
		exit(EXIT_FAILURE);
	}

	int i;
	for (i = 0; i < SIZE; i++)
		fprintf(numberFile, "%d\n", i);

	fclose(numberFile);
}

int getSearchValue()
{
	int searchValue;
	printf("Enter the value to search: ");
	scanf("%d", &searchValue);

	return searchValue;
}

void createSearchThreads(int valueToSearch, struct LinkedList **hits)
{
	if (hits == NULL) {
		puts("Null memory location.");
		exit(EXIT_FAILURE);
	}

	struct ThreadParameter * threadParams[10];
	int i;
	int nextLow = 1;

	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		int high;

		if (i == 9) {
			high = 1000;
		} else {
			int scale = (i + 1) * 100 - nextLow + 1;
			high = (rand() % scale) + nextLow;
		}

		threadParams[i] = newThreadParameter(
			fopen(FILE_NAME, "r"), valueToSearch,
			newSearchRange(nextLow, high), newLinkedList()
			);

		nextLow = high + 1;
		pthread_create(&threadId[i], NULL, &linearSearch, threadParams[i]);
	}

	joinSearchThreads(hits);
}

void joinSearchThreads(struct LinkedList **hits)
{
	int i;
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		pthread_join(threadId[i], (void **) &hits[i]);
	}
}

void displayResult(struct LinkedList **hits)
{
	if (valueFound == 0) {
		puts("Could not find number");
		return;
	}

	if (hits == NULL) {
		puts("Null memory location");
		exit(EXIT_FAILURE);
	}

	int i;
	int foundNumber = 0;
	printf("Number found in: ");
	for (i = 0; i < NUMBER_OF_THREADS; i++) {

		if ((hits[i])->size > 0) {
			foundNumber = 1;
			printList(hits[i], "");
		}
	}

	puts("th position");
}

void *linearSearch(void* params)
{
	struct ThreadParameter *l_params = params;
	int base = (l_params->searchRange)->base;
	int limit = (l_params->searchRange)->limit;
	int cursorPosition;
	int value;


	moveFileCursor(base, l_params->fileToSearch);

	for (cursorPosition = base; cursorPosition <= limit; cursorPosition++) {
		if (valueFound == 1) {
			
			break;
		}

		fscanf(l_params->fileToSearch, "%d", &value);
		if (value == (l_params->valueToSearch)) {
			valueFound = 1;
			insert(l_params->hits, cursorPosition);
			
		}
	}

	fclose(l_params->fileToSearch);
	pthread_exit(l_params->hits);

	return NULL;
}

struct SearchRange* newSearchRange(int base, int limit)
{
	struct SearchRange *ti = malloc(sizeof(struct SearchRange));
	ti->base = base;
	ti->limit = limit;

	return ti;
}

struct ThreadParameter* newThreadParameter(FILE *file,
	int value, struct SearchRange *range, struct LinkedList *list)
{
	if (file == NULL) {
		puts("Could not open file.");
		exit(EXIT_FAILURE);
	}


	struct ThreadParameter *thrdParam = malloc(sizeof(struct ThreadParameter));
	thrdParam->fileToSearch = file;
	thrdParam->valueToSearch = value;
	thrdParam->searchRange = range;
	thrdParam->hits = list;
	return thrdParam;
}

void moveFileCursor(int position, FILE* file)
{
	if (file == NULL) {
		puts("Could not open file.");
		exit(EXIT_FAILURE);
	}

	int currentPosition;
	int *trash = (int *) malloc(sizeof(int));

	for (currentPosition = 1; currentPosition < position; currentPosition++)
		fscanf(file, "%d", trash);

	free(trash);
}
