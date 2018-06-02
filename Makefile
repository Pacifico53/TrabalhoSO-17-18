program: 
	gcc -Wall -Wextra -o notebook main.c command.c parser.c 

debug:
	gcc -Wall -Wextra -g -o notebook main.c command.c parser.c 

clean:
	rm notebook
