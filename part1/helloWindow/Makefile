CC = g++
CFLAGS = -Wall -pedantic -g

INCLUDE = -I /media/adel/1E9A1F939A1F6695/cs-path/gameDev/computerGraphics/openGL-Books/opengl/glad/include
INCLUDE += -I.
INCLUDE += -I./include
INCLUDE += -I./include/glm

LIBS = -lglfw3 -lGL 
LIBS += -lX11 -lpthread -lXrandr -lXi -ldl

SRC = helloWindow.cpp
SRC += /media/adel/1E9A1F939A1F6695/cs-path/gameDev/computerGraphics/openGL-Books/opengl/glad/src/glad.c
SRC += ./shader.cpp

helloWindow:$(SRC)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)