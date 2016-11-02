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
#include "editor.h"
/* flags decides whether to highlight or take out highlight */
void find_replace(char *findstr, char *replacestr, int flags) {
	char comparestr[16];
	int j;
	while(!(row == max_lines && col == len(max_lines))) {
		if (len(row) >= strlen(findstr)) {
                mvwinnstr(pad, row, col, comparestr, strlen(findstr));
                ++col;
		if (col == len(row)) {
                	if ((row == pad_topline + LINES - 1)) {
                        	++pad_topline;
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                        }
                        ++row;
                        col = 0;
                        }
                        if (strcmp(comparestr, findstr) == 0) {
                        	wmove(pad, row, col += strlen(findstr) - 1);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
                                for (j = 0; j < strlen(findstr); j++)
                                	backspace();
				if (flags == 1) {
                                	wstandout(pad);
                                	mvwinsstr(pad, row, col, replacestr);
                                	wstandend(pad);
				}
				else
					mvwinsstr(pad, row, col, replacestr);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
				col += strlen(replacestr);
				if (col >= len(row)) {
					++row;
					col = 0;
				}
                                wmove(pad, row, col);
                                prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
			}
                }
		else {
                        if ((row == pad_topline + LINES - 1)) {
				++pad_topline;
				prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
			}
			++row;
			col = 0;
		}
	}
}
