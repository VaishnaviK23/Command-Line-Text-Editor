#include <stdio.h>
#include <curses.h>
#include "editor.h"

int len(int lineno) {
	int linelen = COLS - 1;

	while (linelen >= 0 && mvwinch(pad, lineno, linelen) == ' ')
		linelen--;
	return linelen + 1;
}

