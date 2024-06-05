GLEW_LIBS=$(shell pkgconf glew --libs)
GLFW_LIBS=$(shell pkgconf glfw3 --libs)

main: main.c StringReader.h OBJLoader.h baseFuncs.h
	gcc -o main main.c StringReader.c OBJLoader.c baseFuncs.c -lm  $(GLEW_LIBS) $(GLFW_LIBS)  -Wall

translate: translate.c
	gcc -o translate translate.c

