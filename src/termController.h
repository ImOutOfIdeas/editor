#ifndef TERMCONTROLLER_H
#define TERMCONTROLLER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

struct TermSize {
    int width;
    int height;
};

// Put terminal into raw mode for custom handling
int enableRawMode();

// Put terminal into cooked mode
int disableRawMode();

// Save cursor position and screen
void saveScreen();
  
// Restore cursor and screen
void restoreScreen();

// Clear screen (Duh)
void clear();

// Set the cursor position
void setCursorPosition(int row, int col);

// Get the cursor position
int getCursorPosition(int* rows, int* cols);

// Gets the width and height of the terminal window
void getWindowSize(struct TermSize* ts);

#endif
