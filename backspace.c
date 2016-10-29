#include <stdio.h>
#include <curses.h>
#include "editor.h"
void backspace () {
        if (col == 0 && row == 0) {
        	if (len(row) == 0)
        		wdeleteln(pad);
                flash();
        }
        else if (col == 0 && row == pad_topline) {
        	if (len(row) == 0)
        		wdeleteln(pad);
        	--pad_topline;
        	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        	if (len(row - 1) == 0) {
        		wmove(pad, row - 1, col = 0);
                	--row;
			prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
			--max_lines;
			return;
		}
                wmove(pad, row - 1, col = len(row - 1));
                --row;
		prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
		--max_lines;
                wdelch(pad);
		prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        }
	else if (col == 0) {
		if (len(row) == 0)
        		wdeleteln(pad);
		if (len(row - 1) == 0) {
        		wmove(pad, row - 1, col = 0);
                	--row;
			prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
			--max_lines;
			return;
		}
		wmove(pad, row - 1, col = len(row - 1));
		prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
		--row;
		--max_lines;
		wdelch(pad);
		prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        }
        else {
                wmove(pad, row, --col);
                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
		wdelch(pad);
		prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        }
}







