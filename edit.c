#include <stdio.h>
#include <curses.h>
#include "undo_stack.h"
#include "editor.h"
extern int row, col;
void edit() {

        int c, tmprow, tmpcol, i = -1, j;
        int copystr[COLS - 1];
        keypad(pad, TRUE);
        // char findstr[16], comparestr[16], replacestr[16];

        while(1){

                wmove(pad, row, col);
                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                c = wgetch(pad);
                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                /* Editor commands */
                switch (c) {

                /* hjkl and arrow keys: move cursor
                * in direction indicated */
                        case 'h':
                        case KEY_LEFT:
                                if (col > 0) {
                                        wmove(pad, row, --col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                else
                                        flash();
                                break;
                        case 'j':
                        case KEY_DOWN:
                                if ((row == LINES - 1 && pad_topline == plines - (LINES - 1)) || row == max_lines)
                                        flash();
                                else if ((row == pad_topline + LINES - 1)) {
                                	++pad_topline;
                                	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        wmove(pad, ++row, col);
                                        if(col > len(row))
                                                wmove(pad, row, col = len(row));
                                       
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                else {
                                        wmove(pad, ++row, col);
                                        if(col > len(row))
                                                wmove(pad, row, col = len(row));
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                break;
                        case 'k':
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
                        case 'l':
                        case KEY_RIGHT:
                                if (col < COLS - 1 && col < len(row)) {
                                        wmove(pad, row, ++col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }

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
                                wdelch(pad);
                                break;

                        /* o: open up a new line and enter input mode */
                        case KEY_IL:
                        case 'o':
                                wmove(pad, ++row, col = 0);
                                ++max_lines;
                                winsertln(pad);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                input();
                                break;

                        /* d: delete current line */
                        case KEY_DL:
                        case 'd':
                                wdeleteln(pad);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                break;
                        /* c: copy current line */
                        case KEY_COPY:
                        case 'c':
                                tmpcol = col;
                                move(row, col = 0);
                                for (i = 0; i < len(row); i++) {
                                        copystr[i] = (mvwinch(pad, row, col) & A_CHARTEXT);
                                                wmove(pad, row, ++col);
                                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                wmove(pad, row, col = tmpcol);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                break;
                        /* ctrl+v: paste copied line at cursor */ 
                        case CTRL('V'):
                        	if (i == -1)
                                        flash();
                                else {
                                	tmprow = row;
                                	tmpcol = col;
                                        wmove(pad, row, col);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        for (j = 0; j < i; j++) {
                                                winsch(pad, copystr[j]);
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
                                                
                                        	}
                                        wmove(pad, row = tmprow, col = tmpcol);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                break;
                        /* p: paste copied line below cursor */
                        case 'p':
                                if (i == -1)
                                        flash();
                                else {
                                        tmpcol = col;
                                        if (row == pad_topline + LINES - 1)
                                                ++pad_topline;
                                        wmove(pad, ++row, col = 0);
                                                winsertln(pad);
                                                ++max_lines;
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        for (j = 0; j < i; j++) {
                                                winsch(pad, copystr[j]);
                                                wmove(pad, row, ++col);
                                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        }
                                        wmove(pad, --row, col = tmpcol);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                break;
                        /* P: paste copied line above cursor */
                        case 'P':
                                if (i == -1)
                                        flash();
                                else {
                                        tmpcol = col;
                                        move(row, col = 0);
                                                winsertln(pad);
                                        for (j = 0; j < i; j++)
                                                waddch(pad, copystr[j]);
                                        move(++row, col = tmpcol);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
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
                        	wclear(pad);
                                break;

                        /* w: write and quit */
                        case 'w':
                                return;
                        /* q: quit without writing */
                        case 'q':
                                endwin();
                                exit(5);
                        default:
                                flash();
                                break;
                }
        }
}
