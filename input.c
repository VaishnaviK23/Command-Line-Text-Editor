#include <stdio.h>
#include <curses.h>
#include "editor.h"
/*
 * Insert mode: accept characters and insert them.
 * End with ^D or EIC
 */ 
void input() {
	int c, i, tab_width = 5;
	standout();
	mvwaddstr(curscr, LINES - 1, COLS - 20, "INPUT MODE");
	standend();
	wmove(pad, row, col);
	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
       
	while(1) {
		wmove(pad, row, col);
                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
		c = wgetch(pad);
		prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
		if (c == CTRL('D') || c == KEY_EIC || c == 27)
			break;
		switch(c) {
			case KEY_DC: 
			case 127:
				wdelch(pad);
				if(col == 0 && len(row) == 0)
					wdeleteln(pad);
				break;
			case KEY_BACKSPACE:
				backspace();
				break;			
                        case KEY_LEFT:
                                if (col > 0) {
                                        wmove(pad, row, --col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                else
                                        flash();
                                break;
                        case KEY_DOWN:
                                if ((row == LINES - 1 && pad_topline == plines - (LINES - 1)) || row == max_lines)
                                        flash();
                                else if ((row == pad_topline + LINES - 1)) {
                                	++pad_topline;
                                	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        wmove(pad, ++row, col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        if(col > len(row)) {
                                                wmove(pad, row, col = len(row));
                                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        }                                     
                               }
                                else {
                                        wmove(pad, ++row, col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        if(col > len(row)) {
                                                wmove(pad, row, col = len(row));
                                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        }
                                }
                                break;                       
                        case KEY_UP:
                                if(row == 0)
                                        flash();
                                else if (row == pad_topline) {
                                        --pad_topline;
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        wmove(pad, --row, col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);

                                }
                                else {
                                        wmove(pad, --row, col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                if(col > len(row)) {
                                        wmove(pad, row, col = len(row));
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                break;
                        case KEY_RIGHT:
                                if (col < COLS - 1 && col < len(row)) {
                                        wmove(pad, row, ++col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }

                                else
                                        flash();
                                break;
           		case KEY_ENTER:
            		case 13:
           			if (row == plines)
           				flash();
           		
           			else if ((row == pad_topline + LINES - 1)) {
           				++pad_topline;
           				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
           				wmove(pad, ++row, col = 0);
           				winsertln(pad); 
           				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);  
           				++max_lines;         				
           			}
           			else {           				          				
           				winsertln(pad);
           				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
           				wmove(pad, ++row, col = 0);
           				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);  
           				++max_lines;
           			}				
           			break;
           		case 9:
           			for(i = 0; i < tab_width; i++) {
           				winsch(pad, 32);
        				if (col == COLS - 1 && row == pad_topline + LINES - 1) {
        					wmove(pad, ++row, col = 0);
        					++pad_topline;
        					++max_lines;
        				}
        				else if (col == COLS - 1) {
        					wmove(pad, ++row, col = 0);
        					++max_lines;
        				}
        				else 
        					wmove(pad, row, ++col);
        				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        			}
           			break;
           		case KEY_HOME:
				pad_topline = 0;
				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
           			wmove(pad, row = 0, col = 0);
           			prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
           			break;
           		case KEY_END:
           			pad_topline = max_lines - (LINES - 1);
           			prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
           			row = max_lines;
           			wmove(pad, row, col = len(row));
           			break;
           		case KEY_PPAGE:
           			if (row != 0 && row == pad_topline)
           				pad_topline -= LINES - 1;
           			wmove(pad, row = pad_topline, col = 0);
           			prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
           			break;
           		case KEY_NPAGE:
           			if(row == pad_topline + LINES - 1)
           				pad_topline += LINES - 1;
           			row = pad_topline + LINES - 1;
           				if (row > max_lines)
           					row = max_lines;
           			wmove(pad, row, col = len(row));
           			prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
           			break; 
           		case KEY_IC:
           			flash();
           			break;

           		default: 
           			winsch(pad, c);
        			if (col == COLS - 1 && row == pad_topline + LINES - 1) {
        				++pad_topline;
        				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        				wmove(pad, ++row, col = 0);
        				++max_lines;
        			}
        			else if (col == COLS - 1) {
        				wmove(pad, ++row, col = 0);
        				++max_lines;
        			}
        			else 
        				wmove(pad, row, ++col);
        			prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        			break;
        	}
        	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
   	}
   	wmove(pad, row, col);
	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
}
