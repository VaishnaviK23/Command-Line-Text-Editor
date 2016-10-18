#include <stdio.h>
#include <curses.h>
#include "editor.h"
/*
 * Insert mode: accept characters and insert them.
 * End with ^D or EIC
 */
void input() {
	int c;

	standout();
	mvaddstr(LINES - 1, COLS - 20, "INPUT MODE");
	standend();
	move(row, col);
	refresh();
	while(1) {
		c = getch();
		if (c == CTRL('D') || c == KEY_EIC)
			break;
		switch(c) {
			case KEY_DC: 
			case 127:
				delch();
				break;
			case KEY_BACKSPACE:
				backspace();
				break;
			case KEY_LEFT:
           			if (col > 0)
               				move(row, --col);
           			else
               				flash();
           			break;
           		case KEY_DOWN:
           			if (row < LINES - 1)
               				move(++row, col);
           			else
               				flash();
           			break;
           		case KEY_UP:
           			if (row > 0)
            				move(--row, col);
           			else
            				flash();
           			break;
           		case KEY_RIGHT:
           			if (col < COLS - 1 && col < len(row))
            				move(row, ++col);
           			else
            				flash();
           			break;
           		case KEY_ENTER:
            		case 13:
           			move(++row, col = 0);
           			break;

           		default: 
           			insch(c);
        			move(row, ++col);
        			if (col == COLS - 1)
        				move(++row, col = 0);
        			break;
        	}
        	refresh();
   	}
   	move(LINES - 1, COLS - 20);
   	clrtoeol();
   	move(row, col);
	refresh();
}
