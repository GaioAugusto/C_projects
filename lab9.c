// Keep track and maintain data - process large CSV files
// Data will be stored in memory using linked lists
// Author: Gaio Moraes dos Santos
// Student number: 1008948123

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <math.h>
#define STRING_MAX 1024

typedef struct node {
	char *name;
	int lines;
	int runtime;
	int memory_usage;

	struct node *next;
} Node;

typedef struct linked_list {
	Node *head;
} LinkedList;

void readInputString(char *, int);
void readInputNumber(int *);
void readInputChar(char *);

LinkedList * newList();
char getCommand();

void handleInsert(LinkedList *);
void handleDelete(LinkedList *);
void handleSearch(LinkedList *);
void handlePrint(LinkedList *);
void handleQuit(LinkedList *);

int main() {
	LinkedList *list = newList();
	char command = '\0';

	printf("Experimental database entry\n\n");

	while (command != 'Q') {
		command = getCommand();

		switch (command) {
			case 'I':
				handleInsert(list);
				break;
			case 'D':
				handleDelete(list);
				break;
			case 'S':
				handleSearch(list);
				break;
			case 'P':
				handlePrint(list);
				break;
			case 'Q':
				handleQuit(list);
				break;
		}
	}

	free(list);
	return 0;
}
    
Node* findNodeByName(LinkedList *list, const char *name) {
    Node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void handleInsert(LinkedList *list) {
    char name[STRING_MAX];
    int name_len;

    printf("\nNumber of characters in file name: ");
    readInputNumber(&name_len);

    printf("File name: ");
    readInputString(name, STRING_MAX);

    int lines, runtime, memory_usage;
    printf("Number of lines in CSV file: ");
    readInputNumber(&lines);
    printf("Experiment runtime (ms): ");
    readInputNumber(&runtime);
    printf("Experiment memory usage (B): ");
    readInputNumber(&memory_usage);

    if (findNodeByName(list, name) != NULL) {
        printf("\nAn entry with that file name already exists.\n");
        return;
    }

    Node *new_node = malloc(sizeof(Node));
    new_node->name = strdup(name);
    new_node->memory_usage = memory_usage;
    new_node->runtime = runtime;
    new_node->lines = lines;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *tail = list->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = new_node;
    }
}


void handleDelete(LinkedList *list) {

    printf("\nEnter file name to delete: ");
    char fileName[1024];
    readInputString(fileName, 1024);

    Node *previous = NULL;
    for (Node *current = list->head; current != NULL; previous = current, current = current->next) {
        if (strcmp(current->name, fileName) == 0) {
            if (previous == NULL) {
                list->head = current->next;
            } 
			else {
                previous->next = current->next;
            }
            printf("Deleting entry for CSV file <%s>\n", fileName);
            free(current->name);
            free(current);
            return;
        }
    }
    printf("An entry for file <%s> does not exist.\n", fileName);
}


void handlePrint(LinkedList *list) {
	
    printf("\nData entries:\n");
    if (list->head == NULL) {
        printf("There are no data entries.\n");
        return;
    }
    for (Node *current = list->head; current != NULL; current = current->next) {
        printf("\nFile <%s>", current->name);
        printf("\nLines: %d", current->lines);
        printf("\nRuntime (ms): %d", current->runtime);
        printf("\nMemory usage (B): %d", current->memory_usage);
        double throughput = (double) current->lines / ((double) current->runtime / 1000);
        printf("\nThroughput: %.2lf\n", throughput);
    }
}


void handleSearch(LinkedList *list) {

    char name[STRING_MAX];
    printf("\nEnter file name to find: ");
    readInputString(name, STRING_MAX);
    Node *current = list->head;
    for (Node *current = list->head; current != NULL; current = current->next) {
    	if (strcmp(current->name, name) == 0) {
			printf("File <%s>\n", current->name);
			printf("Lines: %d\n", current->lines);
			printf("Runtime (ms): %d\n", current->runtime);
			printf("Memory usage (B): %d\n", current->memory_usage);
			double throughput = (double) current->lines / ((double) current->runtime / 1000.0);
			printf("Throughput: %.2lf\n", throughput);
			return;
    	}
	}

    printf("An entry for file <%s> does not exist.\n", name);
}


void handleQuit(LinkedList *list) {

	while(list -> head != NULL){
		Node *temp = list -> head;
		list -> head = list -> head -> next;
		//REPETIR O DELETE

		printf("Deleting entry for CSV file <%s>\n", temp->name);
		free(temp->name);
		free(temp);
	}	
}

void readInputString(char *str, int length) {
	int i = 0;
	char c;

	while (i < length && (c = getchar()) != '\n') {
		str[i++] = c;
	}

	str[i] = '\0';
}

void readInputNumber(int *number) {
	char buf[STRING_MAX];
	readInputString(buf, STRING_MAX);
	*number = (int)strtol(buf, (char **)NULL, 10);
}

void readInputChar(char *character) {
	char buf[2];
	readInputString(buf, 2);
	*character = buf[0];
}

LinkedList * newList() {
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = NULL;
	return list;
}

char getCommand() {
	char command;

	printf("\nSupported commands are:\n");
	printf("  I - Insert\n");
	printf("  D - Delete\n");
	printf("  S - Search\n");
	printf("  P - Print\n");
	printf("  Q - Quit\n");

	printf("\nPlease select a command: ");
	readInputChar(&command);

	while (command != 'I' && command != 'D' && command != 'S' && command != 'P' && command != 'Q') {
		printf("Invalid command <%c>, please select a command: ", command);
		readInputChar(&command);
	}

	return command;
}
