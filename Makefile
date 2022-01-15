build:
	gcc tema3.c functions.c -o bmp -Wall -lm -g

run:
	./bmp

clean:
	rm -f bmp
