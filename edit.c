/*****************************************************************************
 * Copyright (C) Vaishnavi Kulkarni vaishnavikulkarni@hotmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3.0 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include "editor.h"
void edit(char *filename) {

        int c, tmprow, tmpcol, i = -1, j, sig_find = 0, flags, go_to;
        int copystr[COLS - 1];
        keypad(pad, TRUE);
        char findstr[16], replacestr[16];
        
        wmove(pad, 0, 0);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        winsdelln(pad, 3);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        wstandout(pad);
        mvwaddstr(pad, 0, 0, "<h:LEFT> <j:DOWN> <k:UP> <l:RIGHT> <w:SAVE AND QUIT> <q:QUIT>");
        mvwaddstr(pad, 1, 0, "<x:DELETE> <d:DELETE LINE> <c:COPY LINE> <CTRL+X:CUT LINE> <CTRL+V:PASTE LINE AT CURSOR> <p:PASTE BELOW> <P:PASTE ABOVE> ");
        mvwaddstr(pad, 2, 0, "<i:INPUT MODE> <o:INPUT FROM NEW LINE> <f:FIND> <r:REPLACE> <g:GO TO LINE> <t:EXIT FIND/REPLACE> <CTRL+L:CLEAR>");
        wstandend(pad);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        max_lines += 3;
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
                                if(row == 3 && pad_topline == 0)
                                        flash();
                                else if (row == 3) {
                                	--pad_topline;
                                	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
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
                        /* c: copy current line, flags = 0 */
                        case KEY_COPY:
                        case 'c':
                                i = cut_copy(copystr, 0);
                                break;
                        /* ctrl+x: cut current line, flags = 1 */
                        case CTRL('X'):
                                i = cut_copy(copystr, 1);
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
                                        j = 0;
                                        wmove(pad, row = tmprow, col = tmpcol);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                break;
                        /* p: paste copied line below cursor, flags = 0 */
                        case 'p':
				pPaste(copystr, i, 0);
                                break;
                        /* P: paste copied line above cursor, flags = 1 */
                        case 'P':
                                pPaste(copystr, i, 1);
                                break;
                        /* f: find a word */
                        case KEY_FIND:
                        case 'f':
                                wstandout(pad);
				mvwaddstr(pad, 0, COLS - 22, "FIND:");
				wstandend(pad);
				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                if (sig_find == 1) {
                                	wmove(pad, 0, COLS - 16);
                                	prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                	wclrtoeol(pad);
                                	prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                        wmove(pad, row = 3, col = 0);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        find_replace(findstr, replacestr, 0);
                                        wmove(pad, row = 3, col = 0);
                                        prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                sig_find = 1;
                                wmove(pad, 0, COLS - 16);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                echo();
                                wgetstr(pad, findstr);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                noecho();
                                strcpy(replacestr, findstr);
                                flags = 1;
                                find_replace(findstr, replacestr, flags);
                                wmove(pad, row = 3, col = 0);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                break;
                        /* r: replace the found word */
                        case KEY_REPLACE:
                        case 'r':
                        	wmove(pad, 0, COLS - 22);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                wclrtoeol(pad);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                if (!sig_find)
                                        flash();
                                else {
                                	wstandout(pad);
					mvwaddstr(pad, 0, COLS - 22, "REPLACE:");
					wstandend(pad);
					prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        sig_find = 0;
                                        wmove(pad, 0, COLS - 13);
                                	prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                	echo();
                                	wgetstr(pad, replacestr);
                                	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                	noecho();
                                        flags = 1;
                                        find_replace(findstr, replacestr, flags);
                                        wmove(pad, row = 3, col = 0);
                                        prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                break;
                        /* exit find/replace mode */
                        case 't':
                                flags = 0;
                                wmove(pad, row = 3, col = 0);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                strcpy(findstr, replacestr);
                                find_replace(findstr, replacestr, flags);
                                sig_find = 0;
                                wmove(pad, 0, COLS - 22);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                wclrtoeol(pad);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                wmove(pad, row = 3, col = 0);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                break;
                        /* go to a line */
			case 'g':
				wstandout(pad);
				mvwaddstr(pad, 0, COLS - 22, "--GO TO--");
				wstandend(pad);
				prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
				wmove(pad, 3, 0);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
				wscanw(pad, "%d", &go_to);
				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
				if (go_to > max_lines || go_to > plines) {
					flash();
					row = 3;
                                }
				else {
					row = go_to + 2;
				}
				wmove(pad, 0, COLS - 22);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                wclrtoeol(pad);
                                prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                wmove(pad, row, col = 0);
                                prefresh(pad, pad_topline = row, 0, 0, 0, LINES - 1, COLS - 1);
				break;
                        /* ^L: redraw screen */
                        case KEY_CLEAR:
                        case CTRL('L'):
                                wclear(pad);
                                row = 3;
                                col = 0;
                                wstandout(pad);
        			mvwaddstr(pad, 0, 0, "<h:LEFT> <j:DOWN> <k:UP> <l:RIGHT> <w:SAVE AND QUIT> <q:QUIT>");
       				mvwaddstr(pad, 1, 0, "<x:DELETE> <d:DELETE LINE> <c:COPY LINE> <CTRL+X:CUT LINE> <CTRL+V:PASTE LINE AT CURSOR> <p:PASTE BELOW> <P:PASTE ABOVE> ");
        			mvwaddstr(pad, 2, 0, "<i:INPUT MODE> <o:INPUT FROM NEW LINE> <f:FIND> <r:REPLACE> <g:GO TO LINE> <t:EXIT FIND/REPLACE> <CTRL+L:CLEAR>");
        			wstandend(pad);
        			prefresh(pad, pad_topline = 0, 0, 0, 0, LINES - 1, COLS - 1);
                                break;

                        /* w: write and quit */
                        case 'w':
                                return;
                        /* q: quit without writing */
                        case 'q':
                                if(creat)
                                        remove(filename);
                                endwin();
                                exit(5);
                        default:
                                flash();
                                break;
                }
        }
}



