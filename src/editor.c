#include "editor.h"

enum EditorMode {
    NORMAL_MODE,
    INSERT_MODE,
    // Add more modes as needed
};

int row = 1, col = 1;                // Current cursor position
char c;                              // Stores currently pressed character
struct TermSize* ts = NULL;          // Struct for terminal dimensions

void uncookTerm(struct LineNode** head, char* fName) {
    ts = (struct TermSize*)malloc(sizeof(struct TermSize)); // Initialize terminal dimensions struct
    setCursorPosition(row, col);
    getWindowSize(ts);
    createLinkedList(head, fName);
    enableRawMode();
    clear();
}

void cookTerm(struct LineNode** head) {
    clear();
    disableRawMode();
    freeList(*head);    // Free linked list
    free(ts);           // Free terminal size struct
}

void adjustCursorPosition(struct LineNode** head, struct TermSize* ts) {
    //int numLines = getNumberOfLines(*head);
    size_t lineLen = lineLength(*head, row);

    setCursorPosition(ts->height - 1, 0);
    printf("%ld", lineLen);
    setCursorPosition(row, col);

    if (row < 1) row = 1;                           
    else if (row > ts->height) row = ts->height;
    if (col < 1) col = 1;                           
    else if (col > ts->width) col = ts->width;

    // Row checking
    //if (row > numLines){
    //    row = numLines;
    //}

    // Line checking
    if (col > lineLen) {
        col = lineLen;
    }
}

// TODO: make a render loop function (clear, show file (if changed), reposition cursor, update bottom bar
void editorLoop(struct LineNode** head) {
    enum EditorMode mode = NORMAL_MODE; // Start in normal mode
    
    clear();            // Initial screen clear
    printList(*head);   // Show file contents

    while (read(STDIN_FILENO, &c, 1) == 1) {
        switch (mode) {
            case NORMAL_MODE:
                switch(c) {
                    case 'i':
                        mode = INSERT_MODE;
                        printf("\e[5 q"); // Turn cursor into bar
                        fflush(stdout);
                        break;
                    // Basic movement (hjkl)
                    case 'j': // Down
                        setCursorPosition(++row, col);
                        break;
                    case 'k': // Up
                        setCursorPosition(--row, col);
                        break;
                    case 'h': // Left
                        setCursorPosition(row, --col);
                        break;
                    case 'l': // Right
                        setCursorPosition(row, ++col);
                        break; 
                    default:
                        break;
                }

                break;

            case INSERT_MODE:
                switch(c) {
                    case 27:
                        mode = NORMAL_MODE;
                        printf("\e[2 q"); // Turn cursor into block
                        fflush(stdout);
                        break;  
                    case 8:
                        deleteLetter(*head, row, col);
                        setCursorPosition(row, --(col));
                        break;
                    default:
                        insertLetter(*head, c, row - 1, col - 1);
                        setCursorPosition(row, ++col);
                        break;
                }

            default: // No mode? ¯\_(ツ)_/¯
                break;
        }
        
        // Per mode end of loop logic
        if (mode == INSERT_MODE) {
            clear();
            printList(*head);
            setCursorPosition(row, col);
        }

        // General end of loop logic
        adjustCursorPosition(head, ts);

    }
}
