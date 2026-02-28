# to compile and run in one command type:
# make run

# define which compiler to use
CXX		:= g++
OUTPUT	:= sfmlgame
OS		:= $(shell uname)
SRC_DIR	:= ./src

# mac os compiler / linker flags

ifeq ($(OS), Darwin)
	SFML_DIR	:= /opt/homebrew/opt/sfml
	CXX_FLAGS	:= -O3 -std=c++23 -Wno-unused-result -Wno-deprecated-declarations
	INCLUDES	:= -I$(SRC_DIR) -I$(SRC_DIR)/imgui -I$(SRC_DIR)/imgui-sfml -I$(SFML_DIR)/include
	LDFLAGS		:= -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L$(SFML_DIR)/lib -framework OpenGL
endif

# the source files for the ecs game engine
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/imgui/*.cpp $(SRC_DIR)/imgui-sfml/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

# Include dependency files
DEP_FILES := $(OBJ_FILES:.o=.d)
-include $(DEP_FILES)

# All of these targets are made with make
all: $(OUTPUT)

# Define the main executable requirements/commands
$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# Specifies how the object files are compiled from cpp files
%.o: %.cpp
	$(CXX) -MMD -MP -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

# 'make clean' will remove all intermediate build files
clean:
	rm -f $(OBJ_FILES) $(DEP_FILES) ./bin/$(OUTPUT)

# 'make run' will compile and run the program
run: $(OUTPUT)
	cd bin && ./$(OUTPUT) && cd ..