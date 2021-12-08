all:
	gcc uebung1_fork.c -o uebung1_fork -Wall
	gcc uebung1_zombie.c -o uebung1_zombie -Wall
	gcc uebung1_shell.c -g -o uebung1_shell -Wall
	gcc test_print.c -o test_print -Wall
	gcc test_silent.c -o test_silent -Wall

fork: all
	./uebung1_fork

zombie: all
	./uebung1_zombie

shell: all
	./uebung1_shell