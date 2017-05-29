

run: main.o
	g++ main.o -lGL -lGLEW `pkg-config --static --libs glfw3` -o driver

main.o: main.cpp
	g++ -c main.cpp -I /usr/include/GL -I /usr/include/GLFW -std=c++11

clean:
	rm *.o driver
