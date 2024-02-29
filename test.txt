#include <stdio.h>
#include "editor.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct LineNode* head = NULL;

    uncookTerm(&head, argv[1]);
    editorLoop(&head);
    cookTerm(&head);

    return 0;
}

