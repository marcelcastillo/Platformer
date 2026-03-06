CXX := g++
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT := sfmlgame

ifeq ($(OS),Windows_NT)
    PLATFORM := WINDOWS
	SFML_DIR := $(SRC_DIR)/SFML
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := DARWIN
		SFML_DIR := /opt/homebrew/opt/sfml
    else
        PLATFORM := LINUX
    endif
endif

ifeq ($(PLATFORM),WINDOWS)
    TARGET := $(BIN_DIR)/$(OUTPUT).exe
else
    TARGET := $(BIN_DIR)/$(OUTPUT)
endif

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/imgui/*.cpp) $(wildcard $(SRC_DIR)/imgui-sfml/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

CXX_FLAGS := -O3 -std=c++23 -MMD -MP
INCLUDES := -I$(SRC_DIR) -I$(SRC_DIR)/imgui -I$(SRC_DIR)/imgui-sfml -I$(SFML_DIR)/include
LDFLAGS := -L$(SFML_DIR)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

ifeq ($(PLATFORM),DARWIN)
    CXX_FLAGS += -Wno-unused-result -Wno-deprecated-declarations
    LDFLAGS += -framework OpenGL
endif

ifeq ($(PLATFORM),LINUX)
    LDFLAGS += -lGL
endif

ifeq ($(PLATFORM),WINDOWS)
    LDFLAGS += -lopengl32
endif

-include $(DEP_FILES)

all: $(TARGET)

$(TARGET): $(OBJ_FILES) Makefile
ifeq ($(PLATFORM),WINDOWS)
	if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
else
	@mkdir -p $(BIN_DIR)
endif
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(PLATFORM),WINDOWS)
	if not exist "$(dir $@)" mkdir "$(dir $@)"
else
	@mkdir -p $(dir $@)
endif
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

clean:
ifeq ($(PLATFORM),WINDOWS)
	if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR)
	if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)
else
	rm -rf $(OBJ_DIR) $(BIN_DIR)
endif

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run