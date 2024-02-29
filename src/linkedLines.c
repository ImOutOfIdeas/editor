#include "linkedLines.h"

struct LineNode* createNode(char* data) {
    struct LineNode* newNode = (struct LineNode*)malloc(sizeof(struct LineNode));
    newNode->data = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);

    strcpy(newNode->data, data);
    newNode->next = NULL;

    return newNode;
}

void prependLineNode(struct LineNode** head, struct LineNode* newNode) {
    newNode->next = *head;
    *head = newNode;
}

void appendLineNode(struct LineNode** head, struct LineNode* newNode) {
    struct LineNode* here = *head;

    if (*head == NULL) *head = newNode;
    else {
        while(here->next != NULL) here = here->next;
        here->next = newNode;
    }
}

void insertLineNode(struct LineNode** head, struct LineNode* newNode, int index) {
    if (index <= 0) prependLineNode(head, newNode);

    struct LineNode* here = *head;

    while (here->next != NULL && index > 0) { // Will get to min(index, end of list)
        here = here->next;
        index--;
    }

    newNode->next = here->next;
    here->next = newNode;
}

int createLinkedList(struct LineNode** head, const char* fName) {
    FILE* f = fopen(fName, "r");
    long size = 0;
    char line[MAX_LINE_LENGTH];

    if (NULL == f) { // No file pointer returned
        printf("Error opening file\n");
        return -1;
    }

    fseek(f, 0L, SEEK_END); // Point to EOF
    size = ftell(f);	    // Get size of file
    rewind(f);              // Reset file pointer

    while (fgets(line, sizeof(line), f) != NULL) {
        appendLineNode(head, createNode(line));
    }   

    if (ferror(f) != 0) { // Error indicator set
        printf("Error reading file\n");
        return -1;
    }
    if (0 != fclose(f)) {
        printf("Error closing file\n");
        return -1;
    }

    return size;
}

void freeList(struct LineNode* head) {
    struct LineNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void printList(struct LineNode* head) {
    while (head != NULL) {
        printf("%s", head->data);
        head = head->next;
    }

    printf("\n");
}

void insertLetter(struct LineNode* head, char c, int n, int i) {
    for (int j = 0; j < n; j++) head = head->next;

    for (int j = strlen(head->data) - 1; j >= i; j--) {
        head->data[j + 1] = head->data[j];
    }

    head->data[i] = c;
}

void deleteLetter(struct LineNode* head, int n, int i) {
    for (int j = 0; j < n; j++) head = head->next; // Get to n-th node (no index checking)

    for (int j = i; j < strlen(head->data); j++) {
        head->data[j] = head->data[j + 1];
    }
}


size_t getLineLength(struct LineNode* head, int n) {
    for (int i = 0; i <= n; i++) {
        if (head == NULL) return 0;

        if (i == n) return (head != NULL) ? strlen(head->data) : 0;

        head = head->next;
    }

    return 0; // How did you get here
}

size_t getFileLines(struct LineNode* head) {
    if (head == NULL) return 0;
    
    size_t size = 1; // Non-empty file can't have zero lines 
    while (head->next != NULL) {
        size++;
        head = head->next;
    }

    return size;
}

