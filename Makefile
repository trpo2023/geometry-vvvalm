all: Hello Geometry
Hello: Hello.c
	gcc -Wall -Werror -o Hello Hello.c
Geometry: Geometry.c
	gcc -Wall -Werror -o Geometry Geometry.c
cleanH:
	rm Hello
runH:
	./Hello
cleanG:
	rm Geometry
runG:
	./Geometry
