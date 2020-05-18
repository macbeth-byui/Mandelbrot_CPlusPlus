# sudo apt-get install freeglut3
# ldconfig -p  ... this will show which libraries are installed


all: mandelbrot.o drawing.o
	g++ -std=c++11 mandelbrot.o drawing.o -lpthread -lglut -lGLU -lGL -o mandelbrot.exe

mandelbrot.o: mandelbrot.cpp app.h drawing.h
	g++ -std=c++11 -c mandelbrot.cpp -o mandelbrot.o

drawing.o: drawing.cpp drawing.h app.h
	g++ -std=c++11 -c drawing.cpp -o drawing.o

clean:
	-rm -rf *.o *.exe

