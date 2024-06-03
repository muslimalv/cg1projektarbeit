GLEW_LIBS=$(shell pkgconf glew --libs)
GLFW_LIBS=$(shell pkgconf glfw3 --libs)

neu: neu.c StringReader.h
	gcc -o neu neu.c StringReader.c  -lm  $(GLEW_LIBS) $(GLFW_LIBS)  -Wall

warum: warum.c
	gcc -o warum warum.c
