all: Geometry
Geometry: Geometry.c
	gcc -Wall -Werror -o Geometry Geometry.c
cleanG:
	rm Geometry
runG:
	./Geometry
