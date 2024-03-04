#ifndef LINKEDLINES_H
#define LINKEDLINES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

typedef struct LineNode {
  char* data;
  struct LineNode* next;
} LineNode;

LineNode* createNode(char* data);
void prependLineNode(LineNode** head, LineNode* newNode);
void appendLineNode(LineNode** head, LineNode* newNode);
int insertLineNode(LineNode** head, LineNode* newNode, int index);
void freeList(LineNode* head);
void printList(LineNode* head);
int insertLetter(LineNode* head, char letter, int line, int position);
int deleteLetter(LineNode* head, int n, int i);
size_t lineLengthAt(LineNode* head, int n);
size_t lineCount(LineNode* head);
int createLinkedList(LineNode** head, const char* fileName);

#endif

