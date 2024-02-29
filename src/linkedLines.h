#ifndef LINKEDLINES_H
#define LINKEDLINES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100 // Max number of characters on one line

// Holds the data of each line in a linked list
struct LineNode {
    char* data;
    struct LineNode* next;
};

char *strDuplicate(const char *str);

// Create and return a new LineNode
struct LineNode* createNode(char* data);

// Prepend LineNode to linked list
void prependLineNode(struct LineNode** head, struct LineNode* newNode);

// Append LineNode to linked list
void appendLineNode(struct LineNode** list, struct LineNode* newNode);

// Insert node into linked list at certain index
void insertLineNode(struct LineNode** head, struct LineNode* newNode, int index);

// Creates a line linked list representation of a file
int createLinkedList(struct LineNode** head, const char* fName);

// Frees a list and all associated data
// TODO: Pass lineNode** and free the line data as well
void freeList(struct LineNode* head);

// Print the current file buffer
void printList(struct LineNode* head);

// Insert c into n-th node of linked-list head at index i
void insertLetter(struct LineNode* head, char c, int n, int i);

// Delete char c at index i from node n in list head
void deleteLetter(struct LineNode* head, int n, int i);

// Gets length of data from n-th node of linked-list head
size_t getLineLength(struct LineNode* head, int n);

// Gets the number of lines in the file
size_t getFileLines(struct LineNode* head);

#endif
