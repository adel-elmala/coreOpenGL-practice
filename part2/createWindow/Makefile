CC = g++ 
CFLAGS = -Wall -pedantic -g -Wno-unused-but-set-variable -Wno-sign-compare

INCLUDE = -I../Deps/include 

LIBS = -L../Deps/libs
LIBS += ../Deps/libs/libglfw3.a -ldl -lX11 -lXi -lGL -lpthread

SRC = createWin.cpp
SRC += ../Deps/src/glad.c

createWin: $(SRC) 
	@$(CC) $(CFLAGS) $(INCLUDE) $(SRC) $(LIBS) -o ./bin/$@ 

.PHONY:setup,clean,run

setup:
	mkdir -p ./bin

clean:
	rm -r ./bin/*
run:
	./bin/createWin
