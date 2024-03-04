#include "linkedLines.h"

/**
 * @brief Allocates memory for a new line node and initializes its data.
 *
 * This function allocates memory for a new `struct LineNode` and initializes its
 * `data` member with a copy of the provided `data` string. It also sets the
 * `next` pointer to NULL.
 *
 * @param data The string data to be stored in the new line node.
 *
 * @returns A pointer to the newly created line node on success, or NULL on failure.
 *  In case of allocation failure, memory for the node is freed.
 */
struct LineNode* createNode(char* data) {
    struct LineNode* newNode = (struct LineNode*)malloc(sizeof(struct LineNode));
    if (newNode == NULL) {
        return NULL;
    }

    newNode->data = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (newNode->data == NULL) {
        free(newNode);
        return NULL;
    }

    strcpy(newNode->data, data);
    newNode->next = NULL;

    return newNode;
}

/**
 * @brief Prepends a new line node to the beginning of the linked list.
 *
 * This function takes a pointer to the head (`head`) of the linked list and a
 * pointer to a newly created line node (`newNode`). It prepends the `newNode`
 * to the beginning of the list by setting its `next` pointer to the current head
 * and then updating the `head` pointer to point to the `newNode`.
 *
 * @param head A pointer to the head of the linked list.
 * @param newNode A pointer to the new line node to be prepended.
 */
void prependLineNode(struct LineNode** head, struct LineNode* newNode) {
    newNode->next = *head;
    *head = newNode;
}

/**
 * @brief Appends a new line node to the end of the linked list.
 *
 * This function takes a pointer to the head (`head`) of the linked list and a
 * pointer to a newly created line node (`newNode`). It traverses the list to
 * find the last node and then sets the `next` pointer of the last node to
 * point to the `newNode`.
 *
 * @param head A pointer to the head of the linked list.
 * @param newNode A pointer to the new line node to be appended.
 */
void appendLineNode(struct LineNode** head, struct LineNode* newNode) {
    struct LineNode* here = *head;

    if (*head == NULL) {
        *head = newNode;
    } else {
        while (here->next != NULL) {
            here = here->next;
        }
        here->next = newNode;
    }
}

/**
 * @brief Inserts a new line node at a specific index in the linked list.
 *
 * This function takes a pointer to the head (`head`) of the linked list, a
 * pointer to a newly created line node (`newNode`), and an index (`index`).
 * It inserts the `newNode` at the specified `index` in the list. The function
 * handles negative indices by prepending the node and returns 0. It also checks
 * for out-of-bounds indices (beyond the end of the list) and returns -1 on
 * failure.
 *
 * @param head A pointer to the head of the linked list.
 * @param newNode A pointer to the new line node to be inserted.
 * @param index The index at which to insert the new node (0-based).
 *
 * @returns 0 on success, -1 on failure (out-of-bounds index or allocation failure).
 */
int insertLineNode(struct LineNode** head, struct LineNode* newNode, int index) {
    if (index <= 0) {
        prependLineNode(head, newNode);
        return 0;
    }

    struct LineNode* here = *head;
    int i = 0;
    while (here->next != NULL && i < index - 1) { // Traverse to node before target index
        here = here->next;
        i++;
    }

    newNode->next = here->next;
    here->next = newNode;
    return 0;
}

/**
 * @brief Frees all memory allocated for the linked list.
 *
 * This function traverses the linked list starting from the head (`head`) and
 * frees the memory associated with each node. It first frees the memory
 * allocated for the node's data (`data`) and then frees the memory for the node
 * itself (`struct LineNode`). The function iterates until the `head` becomes
 * NULL, signifying the end of the list.
 *
 * @param head A pointer to the head of the linked list to be freed.
 */
void freeList(struct LineNode* head) {
    struct LineNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

/**
 * @brief Prints the content of the entire linked list to the console.
 *
 * This function iterates through the linked list starting from the head (`head`)
 * and prints the content (`data`) of each node to the console. It continues
 * iterating until the `head` becomes NULL, signifying the end of the list.
 *
 * @param head A pointer to the head of the linked list to be printed.
 */
void printList(struct LineNode* head) {
    while (head != NULL) {
        printf("%s", head->data);
        head = head->next;
    }

    printf("\n");
}

/**
 * @brief Inserts a character at a specific position within a line in the linked list.
 *
 * This function takes a pointer to the head (`head`) of the linked list, the line
 * number (`n`) within the list (0-based indexing), the character to be inserted
 * (`c`), and the position (`i`) within the line where the character should be
 * inserted (also 0-based indexing). It then shifts existing characters to make space 
 * for the new character using `memmove`. The function handles out-of-bounds indices by
 * returning -1. 
 *
 * @param head A pointer to the head of the linked list.
 * @param letter The character to be inserted.
 * @param line The line number within the list where the insertion should occur.
 * @param position The position within the line where the character should be inserted.
 *
 * @returns 0 on success and -1 on error
 */
int insertLetter(struct LineNode* head, char letter, int line, int position) {
    if (line < 0 || position < 0) {
        printf("insertLetter: Indices cannot be negative\n");
        return - 1;
    }

    for (int j = 0; j < line; j++) {
        if (head == NULL) {
            printf("insertLetter: Index for line number too large\n");
            return -1;
        }
        head = head->next;
    }

    if (head == NULL || strlen(head->data) < position + 1) {
        printf("insertLetter: Index for line length too large\n");
        return -1;
    }

    size_t length = strlen(head->data);

    memmove(head->data + position + 1, head->data + position, length - position);
    head->data[position] = letter;

    return 0;
}

/**
 * @brief Deletes a character from a specific position within a line in the linked list.
 *
 * This function takes a pointer to the head (`head`) of the linked list, the line
 * number (`line`) within the list (0-based indexing), and the position (`position`) 
 * within the line where the character should be deleted (also 0-based indexing).
 * The function handles out-of-bounds indices by return -1 It then shifts remaining 
 * characters to the left using `memmove` to overwrite the character at the deletion position.
 * Finally, it null-terminates the shortened string.
 *
 * @param head A pointer to the head of the linked list.
 * @param line The line number within the list where the deletion should occur.
 * @param position The position within the line where the character should be deleted.
 *
 * @returns 0 on success -1 on error
 */
int deleteLetter(struct LineNode* head, int line, int position) {
    if (line < 0 || position < 0) {
        printf("deleteLetter: Indices cannot be negative\n");
        return -1;
    }

    for (int j = 0; j < line; j++) {
        if (head == NULL) {
            printf("deleteLetter: Index for line number too large\n");
            return -1;
        }
        head = head->next;
    }

    if (head == NULL || strlen(head->data) <= position) {
        printf("deleteLetter: Index for line length too large\n");
        return -1;
    }

    size_t length = strlen(head->data);

    memmove(head->data + position, head->data + position + 1, length - position - 1);
    head->data[length - 1] = '\0';

    return 0;
}

/**
 * @brief Retrieves the length (number of characters) of the content stored in a specific node of the linked list.
 *
 * This function takes a pointer to the head (`head`) of the linked list and an index (`index`). 
 * It traverses the list to reach the node at the specified `index` (0-based indexing) and returns the length 
 * of the content (`data`) stored within that node. The function handles negative indices by returning 0. 
 * It also checks for indices beyond the end of the list and returns 0 in such cases.
 *
 * @param head A pointer to the head of the linked list.
 * @param index The index of the node in the list whose content length is to be retrieved.
 *
 * @returns The length (number of characters) of the content stored in the node at the specified index, 
 *          or 0 if the index is out-of-bounds.
 */
size_t lineLengthAt(struct LineNode* head, int n) {
    if (n < 0) {
        return 0;
    }

    for (int i = 0; i <= n; i++) {
        if (head == NULL) {
            return 0;
        }
        head = head->next;
    }

    return (head != NULL) ? strlen(head->data) : 0;
}

/**
 * @brief Calculates the total number of lines (nodes) present in the linked list.
 *
 * This function iterates through the linked list starting from the head (`head`) and 
 * increments a counter (`count`) for each node encountered. It returns the final `count`, 
 * which represents the total number of lines (nodes) stored in the linked list.
 *
 * @param head A pointer to the head of the linked list.
 *
 * @returns The total number of lines (nodes) present in the linked list.
 */
size_t lineCount(struct LineNode* head) {
    if (head == NULL) {
        return 0;
    }

    size_t count = 1;
    while (head->next != NULL) {
        count++;
        head = head->next;
    }

    return count;
}

/**
 * @brief Reads a text file line by line and creates a linked list representation.
 *
 * This function takes the file name (`fileName`) as input and attempts to open the file in read mode. 
 * It then uses `fseek` and `ftell` to determine the file size and allocates a temporary buffer (`line`) 
 * to store each line read from the file. It iterates through the file line by line using `fgets`, 
 * creates a new line node (`newNode`) for each line, and appends it to the linked list. The function 
 * handles potential errors like file opening failure, memory allocation failure during node creation, 
 * and errors reading from the file using `ferror`. It closes the file after processing and returns the 
 * total number of lines (bytes) read from the file, or -1 on error.
 *
 * @param head A pointer to the head of the linked list, which will be populated with the lines from the file.
 * @param fileName The name of the text file to be read.
 *
 * @returns The total number of lines (bytes) read from the file on success, or -1 on error.
 */
int createLinkedList(struct LineNode** head, const char* fileName) {
    FILE* f = fopen(fileName, "r");
    long size = 0;
    char line[MAX_LINE_LENGTH];

    if (f == NULL) {
        printf("Error opening file: %s\n", fileName);
        return -1;
    }

    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    rewind(f);

    while (fgets(line, sizeof(line), f) != NULL) {
        // Remove trailing newline (if present)
        line[strcspn(line, "\n") + 1] = '\0';
        struct LineNode* newNode = createNode(line);
        if (newNode == NULL) {
            printf("Memory allocation error while reading file\n");
            fclose(f);
            return -1;
        }
        appendLineNode(head, newNode);
    }

    if (ferror(f) != 0) {
        printf("Error reading file\n");
        fclose(f);
        return -1;
    }

    fclose(f);

    return size;
}
