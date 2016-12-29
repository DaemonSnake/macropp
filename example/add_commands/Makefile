all:
	gcc add_command.c -fPIC -shared -o libA.so
	gcc main.c -L. -lA -Wl,-rpath=./ -ldl -o test

clean:
	rm -f test
	rm -f libA.so

fclean:	clean

re: fclean all
