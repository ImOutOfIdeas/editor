CC = gcc
CFLAGS = -g -Wall -Werror -std=c11

EXE = editor
INPUT = test.txt
SRC_DIR = src

# Get all C files from the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

all: $(EXE)
	./$(EXE) $(INPUT)

$(EXE): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXE)
