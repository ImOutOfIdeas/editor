#include "editor.h"

enum EditorMode {
    NORMAL_MODE,
    INSERT_MODE,
};

static const char * const modeNames[] = {
	[NORMAL_MODE] = "NORMAL_MODE",
	[INSERT_MODE] = "INSERT_MODE",
};

void uncookTerminal(struct LineNode** head, struct TermSize* ts, char* fileName) {
    createLinkedList(head, fileName);
    getWindowSize(ts);

    enableRawMode();

    clear();
    printList(*head);
}

void cookTerminal(struct LineNode** head, struct TermSize* ts) {
    freeList(*head);
    free(ts);

    clear();
    disableRawMode();
}

void adjustCursorPosition(struct LineNode** head, struct TermSize* ts, int* row, int* col) {
    size_t numLines = lineCount(*head);
    size_t lineLength = lineLengthAt(*head, *row - 1);

    if (*row < 1) *row = 1;                           
    else if (*row > ts->height) *row = ts->height;
    if (*col < 1) *col = 1;                           
    else if (*col > ts->width) *col = ts->width;

    if (*row > numLines){
        *row = numLines;
    }

    if (*col > lineLength) {
        *col = lineLength;
    }
}

void drawStatusBar(const char* modeName, const char* fileName, struct TermSize* ts, int row, int col) {
    printf("\x1b[30;47m"); // Set text to black (30) and background to white (47)

    setCursorPosition(ts->height - 1, 0);
    for (int i = 0; i < ts->width; i++) printf(" ");
    fflush(stdout);

    setCursorPosition(ts->height - 1, 0);
    printf(" %d, %d     %s     %s", row, col, modeName, fileName);
    fflush(stdout);
    
    printf("\x1b[0m"); // Reset to default terminal colors (0)
    
    setCursorPosition(row, col);
}

void startEditor(struct LineNode** head, char* fileName) {
    enum EditorMode mode = NORMAL_MODE;
    struct TermSize* ts = NULL;
    ts = (struct TermSize*)malloc(sizeof(struct TermSize));

    int isQuitter = 0;
    int row = 1, col = 1;
    char c;

    uncookTerminal(head, ts, fileName);

    setCursorPosition(row, col);
    drawStatusBar(modeNames[mode], fileName, ts, row, col);

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
                    case 'j':
                        setCursorPosition(++row, col);
                        break;
                    case 'k':
                        setCursorPosition(--row, col);
                        break;
                    case 'h':
                        setCursorPosition(row, --col);
                        break;
                    case 'l':
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
                    case 10:
                        // TODO: GET ENTER WORKING
                        // Remove the substring from current line after cursor (append with \n)
                        // Add the removed substrin to a new node and insert at row
                        struct LineNode* newNode = createNode("\n");
                        insertLineNode(head, newNode, row);
                        setCursorPosition(++row, col);
                        break;
                    case 127:
                        deleteLetter(*head, row - 1, col - 2);
                        setCursorPosition(row, --col);
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

        adjustCursorPosition(head, ts, &row, &col);

        setCursorPosition(row, col);

        drawStatusBar(modeNames[mode], fileName, ts, row, col);
    }

    cookTerminal(head, ts);
}
