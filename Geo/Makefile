TARGET		=Geo1

$(TARGET):	Geo1.o  gl2ps.o 
	g++ -g -o $(TARGET) Geo1.o  gl2ps.o  \
	 `sdl-config --libs` -lGL -lGLU -lgltt -lttf  -lm

Geo1.o:   
	g++ -g  -c Geo1.cpp `sdl-config --cflags`

gl2ps.o:
	cc -g -c gl2ps.c 

clean:
	rm -f $(TARGET) *.o  core
