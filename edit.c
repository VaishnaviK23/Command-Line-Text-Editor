#include <stdio.h>
#include <curses.h>
#include "undo_stack.h"
extern int row, col;
void edit()
{
   
	int c, tmp, i = -1;
	int a[COLS - 1];

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
       			case 'c':
       				tmp = col;
       				i = 0;
       				move(row, col = 0);
       				while (i < COLS - 1 && (mvinch(row, col) & A_CHARTEXT) != ERR) {
       					a[i++] = (mvinch(row, col++) & A_CHARTEXT);
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
       					for (int j = 0; j < i; j++) 
       						insch(a[j]);
       					insch('\n');
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
       					for (int j = 0; j < i; j++) 
       						insch(a[j]);
       					insch('\n');
       					move(row, col = tmp);
       				}
       				break;         						
       			  				     					
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


