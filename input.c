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
#include <curses.h>
#include "editor.h"
/*
 * Insert mode: accept characters and insert them.
 * End with ^D or EIC
 */
void input() {
        int c, i, tab_width = 5, tmpcol, enter_len;
        int enter[COLS - 1];
        wstandout(pad);
        mvwaddstr(pad, 0, COLS - 25, "--INPUT MODE--");
        wstandend(pad);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
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

                                else if(col == 0) {

                                        if ((row == pad_topline + LINES - 1))
                                                ++pad_topline;
                                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        winsertln(pad);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        wmove(pad, ++row, col = 0);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);

                                }
                                else if(col == len(row)) {
                                         if ((row == pad_topline + LINES - 1))
                                                ++pad_topline;
                                        wmove(pad, ++row, col = 0);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        winsertln(pad);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                else {
                                        if ((row == pad_topline + LINES - 1))
                                                ++pad_topline;
                                        tmpcol = col;
                                        enter_len = len(row) - col;
                                        for (i = 0; i < enter_len; i++) {
                                                enter[i] = (mvwinch(pad, row, col) & A_CHARTEXT);
                                                wmove(pad, row, ++col);
                                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        }
                                        wmove(pad, row, tmpcol);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        wclrtoeol(pad);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        wmove(pad, ++row, col = 0);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        winsertln(pad);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        for (i = 0; i < enter_len; i++) {
                                                winsch(pad, enter[i]);
                                                wmove(pad, row, ++col);
                                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                        }
                                        wmove(pad, row, col = 0);
                                        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                }
                                ++max_lines;
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
                                wmove(pad, row = 3, col = 0);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                break;
                        case KEY_END:
                                pad_topline = max_lines - (LINES - 1);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                row = max_lines;
                                wmove(pad, row, col = len(row));
                                break;
                        case KEY_PPAGE:
                                if (row != 3 && row == pad_topline)
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
        wmove(pad, 0, COLS - 25);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        wclrtoeol(pad);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        wmove(pad, row, col);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
}

