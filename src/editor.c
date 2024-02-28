#include "editor.h"

int row = 1, col = 1;                // Current cursor position
char c;                              // Stores currently pressed character
struct TermSize* ts = NULL;          // Struct for terminal dimensions

void uncookTerm(struct LineNode** head, char* fName) {
    enableRawMode();
    clear();
    ts = (struct TermSize*)malloc(sizeof(struct TermSize)); // Initialize terminal dimensions struct
    createLinkedList(head, fName);
    printList(*head);
}

void cookTerm(struct LineNode** head) {
    disableRawMode();
    clear();
    freeList(*head);    // Free linked list
    free(ts);           // Free terminal size struct
}

// TODO: make a render loop function (clear, show file (if changed), reposition cursor, update bottom bar
// Also ensure cursor doesn't go past newline or last line
// Maybe use Enum for Modes?


// TODO: For Normal and insert mode functions. Fix to recognize and use the static variables
void normalMode(struct LineNode** head) {
    getCursorPosition(&row, &col);
    getWindowSize(ts);

    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        // TODO: Move cursor postion checking to the main render loop
        if (row < 1) row = 1;                         // Top bound
        else if (row > ts->height) row = ts->height;  // Bottom bound
        if (col < 1) col = 1;                         // Left bound
        else if (col > ts->width) col = ts->width;    // Right bound
        else if (col > lineLength(*head, col - 1)) col = lineLength(*head, col - 1);

        switch(c) {
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
                // Mode Changes
            case 'i': // Insertion Mode
                insertMode(*head);
                break;
            default:
                break;
        }
    }
}

void insertMode(struct LineNode* head) {
    printf("\e[5 q"); // Turn cursor into bar
    fflush(stdout);

    while (read(STDIN_FILENO, &c, 1) == 1 && c != 27) {
        switch(c) {
            case 8:
                deleteLetter(head, row, col);
                setCursorPosition(row, --(col));
                break;
            default:
                insertLetter(head, c, row - 1, col - 1);
                setCursorPosition(row, ++col);
                break;
        }
        
        // Draw Cycle for insert mode
        clear();
        printList(head);

        setCursorPosition(ts->height, 0);

        printf("%d, %d", row, col);
        fflush(stdout);

        setCursorPosition(row, col);
    }

    setCursorPosition(row, col);

    printf("\e[2 q"); // Turn cursor back into block
    fflush(stdout);
}

void editorLoop(struct LineNode** head) {
    normalMode(head);

    return;
}
