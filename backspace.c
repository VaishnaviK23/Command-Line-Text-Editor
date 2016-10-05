#include<stdio.h>
#include<curses.h>
void backspace () {
	if (col == 0)
				move(row-1, len(row-1));
			else
				move(row, --col);
			delch();
}
