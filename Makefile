project: main.o edit.o input.o backspace.o linelen.o cut_copy.o pPaste.o find_replace.o
	cc main.o edit.o input.o backspace.o linelen.o cut_copy.o pPaste.o find_replace.o -lncurses -o project
main.o: main.c editor.h
	cc -Wall -c main.c
edit.o: edit.c editor.h
	cc -Wall -c edit.c
input.o: input.c editor.h
	cc -Wall -c input.c
backspace.o: backspace.c editor.h
	cc -Wall -c backspace.c
linelen.o: linelen.c editor.h
	cc -Wall -c linelen.c
cut_copy.o: cut_copy.c editor.h
	cc -Wall -c cut_copy.c
pPaste.o: pPaste.c editor.h
	cc -Wall -c pPaste.c
find_replace.o: find_replace.c editor.h
	cc -Wall -c find_replace.c
