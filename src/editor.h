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

// Handles single character input
void normalMode(struct LineNode** head);

// Enters insert mode at current cursor position
void insertMode(struct LineNode* head);

void editorLoop(struct LineNode** head);
#endif
