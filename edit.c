#include <stdio.h>
#include <curses.h>
#include "undo_stack.h"
#include "editor.h"
extern int row, col;
void edit() {
   
	int c, tmp, i = -1;
	int copystr[COLS - 1];
	char findstr[16], comparestr[16]; replacestr[16];

   	while(1){
   	
       		move(row, col);
       		refresh();
       		c = getch();

       		/* Editor commands */
       		switch (c) {

       		/* hjkl and arrow keys: move cursor
        	* in direction indicated */
       			case 'h':
       			case KEY_LEFT:
           			if (col > 0)
               				col--;
           			else
               				flash();
           			break;

       			case 'j':
       			case KEY_DOWN:
           			if (row < LINES - 1)
               				row++;
           			else
               				flash();
           			break;

       			case 'k':
       			case KEY_UP:
           			if (row > 0)
               				row--;
           			else
               				flash();
           			break;

       			case 'l':
       			case KEY_RIGHT:
           			if (col < COLS - 1)
               				col++;
           			else
               			flash();
           			break;
       			/* i: enter input mode */
      			case KEY_IC:
       			case 'i':
           			input();
           			break;

       			/* x: delete current character */
       			case KEY_DC:
       			case 'x':
           			delch();
           			break;

       			/* o: open up a new line and enter input mode */
       			case KEY_IL:
       			case 'o':
           			move(++row, col = 0);
           			insertln();
           			input();
           			break;

       			/* d: delete current line */
       			case KEY_DL:
       			case 'd':
           			deleteln();
           			break;
           		/* c: copy current line */
       			case KEY_COPY:
				case 'c':
       				tmp = col;
       				move(row, col = 0);
       				for (i = 0; i < len(row); i++) {
       					copystr[i] = (mvinch(row, col) & A_CHARTEXT);
						move(row, ++col);
       				}
       				move(row, col = tmp);
       				break; 
       			/* p: paste copied line below cursor */
       			case 'p':
       				if (i == -1)
       					flash();
       				else {
       					tmp = col;
       					move(++row, col = 0);
						insertln();
       					for (int j = 0; j < i; j++) {
       						insch(copystr[j]);
							move(row, ++col);
						}
       					move(--row, col = tmp);
       				}
       				break; 
       			/* P: paste copied line above cursor */
       			case 'P':
       				if (i == -1)
       					flash();
       				else {
       					tmp = col;
       					move(row, col = 0);
						insertln();
       					for (int j = 0; j < i; j++) 
       						addch(copystr[j]);
       					move(++row, col = tmp);
       				}
       				break; 
       			/* f: find a word */
       			/*case KEY_FIND:
       			case 'f':
       				standout();
					mvaddstr(LINES - 1, COLS - 23, "FIND:");
					standend();
					move(row, col);
					refresh();
       				getstr(findstr);
       				standout();
					mvaddstr(LINES - 1, COLS - 17, findstr);
					standend();
					move(row = 0; col = 0);
					i = 0;
					while(1) {
						comparestr[i] = (mvinch(row, col) & A_CHARTEXT);
						move(row, ++col);
						if (col == len(row))
							move(++row, col = 0);
						i++;
				*/
       			  				     					
       			/* ^L: redraw screen */
       			case KEY_CLEAR:
       			case CTRL('L'):
           			wrefresh(curscr);
           			break;

       			/* w: write and quit */
       			case 'w':
           			return;
       			/* q: quit without writing */
       			case 'q':
           			endwin();
           			exit(2);
       			default:
           			flash();
           			break;
       		}
   	}
}


