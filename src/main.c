#include <stdio.h>
#include "editor.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct LineNode* head = NULL;

    startEditor(&head, argv[1]);

    return 0;
}

