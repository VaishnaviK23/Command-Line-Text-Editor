#include <stdio.h>
#include <error.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "editor.h"
int main(int argc, char *argv[]) {
   	int i, n, l;
   	int c;
   	int line = 0;
   	FILE *fp;

   	if (argc < 2) {
   
       	fprintf(stderr, "Usage: %s file\nHelp option: %s -h", argv[0], argv[0]);
       	exit(1);
   	}
	
	if(strcmp(argv[1], "-h") == 0) {
		printf("Usage: %s FILE\nEDITOR COMMANDS:\nCTRL(D)/KEY_EIC     -Escape input mode and enter command mode\n'h'                 -Move left\n'j'                 -Move down\n'k'                 -Move up\n'l'                 -Move right\n'i'/KEY_IC          -Input mode\n'x'/KEY_DC          -Delete current character\n'o'/KEY_IL          -Open new line and enter input mode\n'd'/KEY_DL          -Delete current line\n'c'/KEY_COPY        -Copy current line\n'p'                 -Paste copied line below the cursor\n'P'                 -Paste copied line above the cursor\nCTRL(L)/KEY_CLEAR   -Clear screen\n'w'                 -Save and quit\n'q'                 -Quit without saving\n", argv[0]);
		exit(2);
	}
	if(strcmp(argv[1], "--h") == 0) {
		fprintf(stderr, "Usage: %s -h\n", argv[0]);
       		exit(3);
       	}
     
   	fp = fopen(argv[1], "a+");
   	if (fp == NULL) {
   
       		fprintf(stderr, "Bad file\n");
       		exit(4);
   	}

   	initscr();
   	cbreak();
   	nonl();
   	noecho();
   	idlok(stdscr, TRUE);
   	keypad(stdscr, TRUE);

   	/* Read in the file */
   	while ((c = getc(fp)) != EOF) {
       		if (c == '\n')
           		line++;
       		if (line > LINES - 2)
           		break;
       		addch(c);
   	}

   	move(0,0);
   	refresh();
   	edit();

   	/* Write out the file */
   	for (l = 0; l < LINES - 1; l++) {
       		n = len(l);
       		for (i = 0; i < n; i++)
           		putc(mvinch(l, i) & A_CHARTEXT, fp);
       		putc('\n', fp);
   	}
   	endwin();
   	return 0;
}


