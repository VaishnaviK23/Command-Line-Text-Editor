#ifndef _EDITOR_H
#define _EDITOR_H

#define CTRL(c) ((c) & 037)

/* Global value of current cursor position */
int row, col;
int len(int);
void edit(void);
void input(void);
void backspace(void);

#endif
