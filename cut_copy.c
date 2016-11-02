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
/* cut/copy current line depending on value of flags */
int cut_copy(int *copystr, int flags) {
	int i, tmpcol;
	tmpcol = col;
        wmove(pad, row, col = 0);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        for (i = 0; i < len(row); i++) {
        	copystr[i] = (mvwinch(pad, row, col) & A_CHARTEXT);
        	wmove(pad, row, ++col);
        	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        }
        wmove(pad, row, col = tmpcol);
        prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        if (flags) {
        	wdeleteln(pad);
        	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
        }
        return i;
}
