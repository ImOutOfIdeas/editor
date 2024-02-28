#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include "termController.h"
#include "linkedLines.h"

// Ready terminal for TUI
void uncookTerm(struct LineNode** head, char* fName);

// Return to normie terminal
void cookTerm(struct LineNode** head);

// Function to perform cursor boundary and in-line checking
void adjustCursorPosition(struct LineNode** head, struct TermSize* ts);

// Handles each modes logic, modal changes, and the draw cycle
void editorLoop(struct LineNode** head);
#endif
