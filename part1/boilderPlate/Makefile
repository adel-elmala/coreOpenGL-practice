CC = g++
CFlags = -msse2 -Wpedantic -g

Include = -I../../common/include
Include += -I../../common/3rdLibs/glad/include
# Include += -I../../common/3rdLibs
# Include += -I../../common/3rdLibs/glm

Libs = -lglfw3 -lGL
Libs += -lX11 -lpthread -lXrandr -lXi -ldl
 

SRC = 	boilerPlate.cpp 
SRC +=	../../common/src/windowIO.cpp
SRC +=	../../common/3rdLibs/glad/src/glad.c
SRC +=	../../common/src/shader.cpp


boilerPlate:$(SRC)
	$(CC) $(CFlags) $(Include) $(SRC) -o ./bin/$@ $(Libs)



.PHONY:clean,run 

run:
	./bin/boilerPlate
clean:
	rm bin/*.o bin/*.out
