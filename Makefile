

run: main.o
	g++ main.o -lGL -lGLEW `pkg-config --static --libs glfw3` -o driver

main.o: main.c
	g++ -c main.c -I /usr/include/GL -I /usr/include/GLFW

clean:
	rm *.o driver
