#include <stdio.h>
#include <curses.h>
#include <stdlib.h>

#define CTRL(c) ((c) & 037)

/* Global value of current cursor position */
int row, col;
int len(int);
void edit(void);
void input(void);
int main(int argc,char *argv[]) {
	extern void perror(), exit();
   	int i, n, l;
   	int c;
   	int line = 0;
   	FILE *fp;

   	if (argc != 2) {
   
       	fprintf(stderr, "Usage: %s file\n", argv[0]);
       	exit(1);
   	}

   	fp = fopen(argv[1], "r");
   	if (fp == NULL) {
   
       	perror(argv[1]);
       	exit(2);
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
   	fclose(fp);

   	move(0,0);
   	refresh();
   	edit();

   	/* Write out the file */
   	fp = fopen(argv[1], "w");
   	for (l = 0; l < LINES - 1; l++) {
       	n = len(l);
       	for (i = 0; i < n; i++)
           	putc(mvinch(l, i) & A_CHARTEXT, fp);
       	putc('\n', fp);
   	}
   	fclose(fp);

   	endwin();
   	return 0;
}


