#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include "termController.h"
#include "linkedLines.h"

void uncookTerminal(struct LineNode** head, struct TermSize* ts, char* fileName);
void cookTerminal(struct LineNode** head, struct TermSize* ts);
void adjustCursorPosition(struct LineNode** head, struct TermSize* ts, int* row, int* col);
void drawStatusBar(const char* modeName, struct TermSize* ts, int row, int col);
void startEditor(struct LineNode** head, char* fileName);

#endif
