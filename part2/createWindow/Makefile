CC = g++ 
EXE = createWin
CFLAGS = -Wall -pedantic -g -Wno-unused-but-set-variable -Wno-sign-compare

INCLUDE = -I../Deps/include 
INCLUDE += -I./dearImGui
IMGUI_DIR = ./dearImGui

LIBS = -L../Deps/libs
LIBS += ../Deps/libs/libglfw3.a -ldl -lX11 -lXi -lGL -lpthread

SRC = createWin.cpp \
	../Deps/src/glad.c\
	./dearImGui/imgui_widgets.cpp\
	./dearImGui/imgui.cpp ./dearImGui/imgui_impl_opengl3.cpp\
	./dearImGui/imgui_demo.cpp ./dearImGui/imgui_draw.cpp\
	./dearImGui/imgui_tables.cpp ./dearImGui/imgui_impl_glfw.cpp


OBJS = $(addsuffix .o, $(basename $(notdir $(SRC))))
OBJSBins = $(addprefix ./bin/, $(OBJS))


all: $(EXE)
	@echo Build complete for $(EXE)

$(EXE): $(OBJSBins)
	$(CXX) $(CFLAGS) $(INCLUDE) -o ./bin/$@ $^ $(LIBS)

./bin/%.o:%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<

./bin/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<

./bin/%.o:../Deps/src/%.c
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<


# createWin: $(SRC) 
# 	@$(CC) $(CFLAGS) $(INCLUDE) $(SRC) $(LIBS) -o ./bin/$@ 

.PHONY:setup,clean,run

setup:
	mkdir -p ./bin

clean:
	rm -r ./bin/*
run:
	./bin/createWin

