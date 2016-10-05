#include <stdio.h>
#include <curses.h>
/*
 * Insert mode: accept characters and insert them.
 * End with ^D or EIC
 */
extern int row, col;
void input()
{
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
			case KEY_DC: case 127:
				delch();
				break;
			case KEY_BACKSPACE:
				backspace();
				break;
			case KEY_LEFT:
           		if (col > 0)
               		col--;
           		else
               		flash();
           		break;
           	case KEY_DOWN:
           		if (row < LINES - 1)
               		row++;
           		else
               		flash();
           		break;
           	case KEY_UP:
           		if (row > 0)
               		row--;
           		else
               		flash();
           		break;
           	case KEY_RIGHT:
           		if (col < COLS - 1)
               		col++;
           		else
               		flash();
           		break;
           	default: 
           		insch(c);
        		move(row, ++col);
        		break;
        }
        refresh();
			
		/*if (c == CTRL('D') || c == KEY_EIC)
			break;
		else if (c == KEY_DC || c == 127) {
			delch();
		}
		else if (c == KEY_BACKSPACE || c == 127) {
			if (col == 0)
				move(row-1, len(row-1));
			else
				move(row, --col);
			delch();
		}
		else {
        		insch(c);
        		move(row, ++col);
		}
		refresh();*/
   	}
   	move(LINES - 1, COLS - 20);
   	clrtoeol();
   	move(row, col);
   	refresh();
	}
