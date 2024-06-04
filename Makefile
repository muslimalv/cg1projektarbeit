GLEW_LIBS=$(shell pkgconf glew --libs)
GLFW_LIBS=$(shell pkgconf glfw3 --libs)

main: main.c StringReader.h 
	gcc -o main main.c StringReader.c  -lm  $(GLEW_LIBS) $(GLFW_LIBS)  -Wall


