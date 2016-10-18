try: main.o edit.o input.o backspace.o linelen.o
	cc main.o edit.o input.o backspace.o linelen.o -lncurses -o try
main.o: main.c editor.h
	cc -Wall -c main.c
edit.o: edit.c undo_stack.h editor.h
	cc -Wall -c edit.c
input.o: input.c editor.h
	cc -Wall -c input.c
backspace.o: backspace.c editor.h
	cc -Wall -c backspace.c
linelen.o: linelen.c editor.h
	cc -Wall -c linelen.c
