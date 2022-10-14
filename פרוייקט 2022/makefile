assembler: main.o PreAssembler.o helpFunc.o assembler.o firstPass.o passFunc.o secondPass.o
	gcc -Wall -ansi -pedantic -g helpFunc.o PreAssembler.o main.o assembler.o secondPass.o firstPass.o passFunc.o -lm -o assembler 
main.o: main.c data.h global_vars.h definitions.h
	gcc -c -Wall -ansi -pedantic -g main.c -o main.o
PreAssembler.o: PreAssembler.c data.h global_vars.h definitions.h
	gcc -c -Wall -ansi -pedantic -g PreAssembler.c -o PreAssembler.o
helpFunc.o: helpFunc.c data.h global_vars.h definitions.h
	gcc -c -Wall -ansi -pedantic -g helpFunc.c -o helpFunc.o
firstPass.o: data.h global_vars.h definitions.h firstPass.c
	gcc -c -Wall -ansi -pedantic -g firstPass.c -o firstPass.o
assembler.o: data.h definitions.h assembler.c 
	gcc -c -Wall -ansi -pedantic -g assembler.c -o assembler.o
passFunc.o: data.h global_vars.h definitions.h assembler.h passFunc.c
	gcc -c -Wall -ansi -pedantic -g passFunc.c -o passFunc.o
secondPass.o: data.h global_vars.h definitions.h assembler.h secondPass.c
	gcc -c -Wall -ansi -pedantic -g secondPass.c -o secondPass.o	
