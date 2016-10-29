#include <stdio.h>
#include <error.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "editor.h"
int main(int argc, char *argv[]) {
   	int i = 0, n, l;
   	int c;
	
   	FILE *fp;

   	if (argc > 2) {
   
       	fprintf(stderr, "Usage: %s <file name>\nHelp option: %s -h", argv[0], argv[0]);
       	exit(1);
   	}
   	
   	if (argc == 1) {
   		initscr();
   		cbreak();
   		nonl();
   		noecho();
   		idlok(stdscr, TRUE);
   		keypad(stdscr, TRUE);

   		mvaddstr(row = LINES/2 - 4, col = COLS/2 - 7, "VUK TEXT EDITOR");
   		mvaddstr(row = LINES/2 - 1, col = COLS/2 - 11, "Mini Project Submission");
   		mvaddstr(row = LINES/2, col = COLS/2 - 10, "by Vaishnavi Kulkarni");
   		mvaddstr(row = LINES/2 + 3, col = COLS/2 - 8, "type <h> : help.");
   		mvaddstr(row = LINES/2 + 4, col = COLS/2 - 8, "type <q> : exit.");
   		while(1) {
   		
   			c = getch();
   			switch (c) {
   				case 'h': 
   					clear();
   					printw("Usage: %s FILE\nEDITOR COMMANDS:\nCTRL(D)/KEY_EIC/Esc -Escape input mode and enter command mode\n'h'                 -Move left\n'j'                 -Move down\n'k'                 -Move up\n'l'                 -Move right\n'i'/KEY_IC          -Input mode\n'x'/KEY_DC          -Delete current character\n'o'/KEY_IL          -Open new line and enter input mode\n'd'/KEY_DL          -Delete current line\n'c'                 -Copy current line\n'p'                 -Paste copied line below the cursor\n'P'                 -Paste copied line above the cursor\nCTRL(L)/KEY_CLEAR   -Clear screen\n'w'                 -Save and quit\n'q'                 -Quit without saving\n\n\nTo exit help, press <q>", argv[0]);
   					break;
   				case 'q':
   					endwin();
   					exit(2);
   				default:
   					flash();
   					break;
   			}
		}   			
   	}
	
	if(strcmp(argv[1], "-h") == 0) {
		printf("Usage: %s FILE\nEDITOR COMMANDS:\nCTRL(D)/KEY_EIC/Esc -Escape input mode and enter command mode\n'h'                 -Move left\n'j'                 -Move down\n'k'                 -Move up\n'l'                 -Move right\n'i'/KEY_IC          -Input mode\n'x'/KEY_DC          -Delete current character\n'o'/KEY_IL          -Open new line and enter input mode\n'd'/KEY_DL          -Delete current line\n'c'                 -Copy current line\n'p'                 -Paste copied line below the cursor\n'P'                 -Paste copied line above the cursor\nCTRL(L)/KEY_CLEAR   -Clear screen\n'w'                 -Save and quit\n'q'                 -Quit without saving\n", argv[0]);
		exit(3);
	}
	if(strcmp(argv[1], "--h") == 0) {
		fprintf(stderr, "Usage: %s -h\n", argv[0]);
       		exit(4);
       	}
     	fp = fopen(argv[1], "a+");
	if (fp == NULL)
		fp = fopen(argv[1], "r");
   	
	while ((c = getc(fp)) != EOF) {
		i++;
		if(i == COLS - 1 || c == '\n') {
			i = 0;
			max_lines++;
		}
	}

	initscr();
   	cbreak();
   	nonl();
   	noecho();
   	idlok(stdscr, TRUE);
   	keypad(stdscr, TRUE);

	plines = 128 * max_lines;
	
	pad = newpad(plines, COLS);
	
	pad_topline = 0;
	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
   	/* Read in the file */
	rewind(fp);
   	while ((c = getc(fp)) != EOF) {
       		pechochar(pad, c);
   	}

   	wmove(pad, row = 0, col = 0);
   	prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
   	edit();
   	fclose(fp);

   	/* Write out the file */
   	fp = fopen(argv[1], "w");
   	for (l = 0; l <= max_lines; l++) {
       		n = len(l);
       		for (i = 0; i < n; i++)
           		putc(mvwinch(pad, l, i) & A_CHARTEXT, fp);
       		putc('\n', fp);
		prefresh(pad, pad_topline, 0, 0, 0, LINES - 1, COLS - 1);
   	}
   	endwin();
   	fclose(fp);
   	return 0;
}

