#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
typedef struct undo_stack {
	char command[10];
	int i;
}undo_stack;
void undo_init(undo_stack *);
void undo_push(undo_stack *, char);
char undo_pop(undo_stack *);
int undo_empty(undo_stack *);
int undo_full(undo_stack *);
