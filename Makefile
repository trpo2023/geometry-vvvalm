all: Hello
Hello: Hello.c
	gcc -Wall -Werror -o Hello Hello.c
clean:
	rm Hello
run:
	./Hello
