#include "termController.h"

struct termios term;

int enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        return -1;
    }

    struct termios raw = term; // Save copy of terminal to make raw
    raw.c_lflag &= ~(ECHO | ICANON); // Make it raw

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

void setCursorPosition(int row, int col) {
    printf("\x1b[%d;%dH", row, col);
    fflush(stdout);
}

// Generated with GPT 3.5 (2/22/24)
int getCursorPosition(int *rows, int *cols) {
    // Request cursor position
    write(STDOUT_FILENO, "\x1b[6n", 4);

    // Read response from the terminal
    char buf[32];
    int i = 0;
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) {
            break;
        }
        if (buf[i] == 'R') {
            break;
        }
        i++;
    }
    buf[i] = '\0';

    // Parse the response to get cursor position
    if (sscanf(buf, "\x1b[%d;%dR", rows, cols) != 2) {
        perror("sscanf");
        return -1;
    }

    return 0; 
}

void saveScreen() {
    printf("\e[?1049h");  // Save cursor and screen
}

void restoreScreen() {
    printf("\e[?1049l");  // Restore cursor and screen
}

void clear(){
    write(STDOUT_FILENO, "\x1b[2J", 4); // Clear the screen
    write(STDOUT_FILENO, "\x1b[H", 3);  // Reset cursor to the top-left corner
}

void getWindowSize(struct TermSize *ts) {
    struct winsize ws;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) {
        ts->height = ws.ws_row;
        ts->width = ws.ws_col;
    }
}
