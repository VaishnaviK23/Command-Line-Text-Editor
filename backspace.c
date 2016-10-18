#include <stdio.h>
#include <curses.h>
#include "editor.h"
void backspace () {
	if (col == 0 && row == 0)
		flash();
	else if (col == 0) {
		move(row-1, col = len(row-1) - 1);
		--row;
		delch();
	}
	else {
		move(row, --col);
		delch();
	}
}
