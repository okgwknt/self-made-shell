oksh: oksh.c
	gcc -o oksh oksh.c

.PHONY: clean
clean: 
	rm -f oksh 