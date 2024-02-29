#include "editor.h"

enum EditorMode {
    NORMAL_MODE,
    INSERT_MODE,
    // Add more modes as needed
};

static const char * const modeNames[] = {
	[NORMAL_MODE] = "NORMAL_MODE",
	[INSERT_MODE] = "INSERT_MODE",
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

void adjustCursorPosition(struct LineNode** head) {
    size_t numLines = getFileLines(*head);
    size_t lineLength = getLineLength(*head, row);

    if (row < 1) row = 1;                           
    else if (row > ts->height) row = ts->height;
    if (col < 1) col = 1;                           
    else if (col > ts->width) col = ts->width;

    // Row checking
    if (row > numLines){
        row = numLines;
    }

    // Line checking
    if (col > lineLength) {
        col = lineLength;
    }
}

void drawStatusBar(const char* modeStr) {
    printf("\x1b[30;47m"); // Set text to black (30) and background to white (47)

    // Draw the white bar across the bottom
    setCursorPosition(ts->height - 1, 0);
    for (int i = 0; i < ts->width; i++) printf(" ");
    fflush(stdout);

    // Draw the information in the bar
    setCursorPosition(ts->height - 1, 0);
    printf(" %d, %d     %s", row, col, modeStr);
    fflush(stdout);
    
    printf("\x1b[0m"); // Reset colors to default (0)
    
    setCursorPosition(row, col);    // Keep cursor where you left it
}

void editorLoop(struct LineNode** head) {
    enum EditorMode mode = NORMAL_MODE; // Start in normal mode
    int isQuitter = 0;

    clear();                        // Initial screen clear
    printList(*head);               // Show file contents
    setCursorPosition(row, col);    // Set cursor to top left
    drawStatusBar(modeNames[mode]);                // Initial status bar draw

    while (isQuitter == 0 && read(STDIN_FILENO, &c, 1) == 1) {
        setCursorPosition(row, col); // Updates cursor every cycle

        switch (mode) {
            case NORMAL_MODE:
                switch(c) {
                    case 'q':
                        isQuitter = 1;
                        break;
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
                    case 127:
                        deleteLetter(*head, row - 1, col);
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

        clear();
        printList(*head);

        adjustCursorPosition(head);     // Adjust and put at valid location (if needed)

        drawStatusBar(modeNames[mode]);

    }
}
