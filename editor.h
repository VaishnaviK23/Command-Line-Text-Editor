#ifndef __EDITOR_H
#define __EDITOR_H

#define CTRL(c) ((c) & 037)

WINDOW *pad;

/* Global value of current cursor position */
int row, col;
/*Global value of total number of lines*/
int max_lines;
/*Global value of topmost line in pad*/
int pad_topline;
long int plines;
 
int len(int);
void edit(void);
void input(void);
void backspace(void);
void file_lines(FILE *);

#endif
