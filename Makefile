shell: libparser.a
	gcc -L. -o shell shell.c -lparser -lhandler
libparser.a: parser.o
	ar -cvq libhandler.a handler.o
	ar -cvq libparser.a parser.o
parser.o:
	gcc -c handler.c
	gcc -c parser.c
clean:
	rm -f shell a.out lib*.a *.o 
