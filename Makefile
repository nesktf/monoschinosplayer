#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-i686-SDL2
#
PROJECT_NAME = MonosChinosPlayer

SOURCE_DIR = src
LIB_DIR = lib
BUILD_DIR = build
EXE = $(BUILD_DIR)/$(PROJECT_NAME)

IMGUI_DIR = $(LIB_DIR)/imgui

SOURCES = \
	$(SOURCE_DIR)/main.cpp \
	$(SOURCE_DIR)/gui.cpp \
	$(SOURCE_DIR)/mpv.cpp \
	$(SOURCE_DIR)/util.cpp \
	$(SOURCE_DIR)/layer/layer_base.cpp \
	$(SOURCE_DIR)/layer/layer_explore.cpp \
	$(SOURCE_DIR)/layer/layer_eplist.cpp \
	$(SOURCE_DIR)/layer/layer_player.cpp \
	$(SOURCE_DIR)/layer/layer_search.cpp \
	$(SOURCE_DIR)/extractor/extr_base.cpp \
	$(SOURCE_DIR)/extractor/extr_jkanime.cpp \
	$(SOURCE_DIR)/extractor/extr_pelisplushd.cpp \
	$(SOURCE_DIR)/widgets/widget_spinner.cpp \
	$(SOURCE_DIR)/widgets/widget_animegrid.cpp

SOURCES += \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_sdl.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp \
	$(IMGUI_DIR)/misc/cpp/imgui_stdlib.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o , $(basename $(notdir $(SOURCES)))))


CXX = g++
CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(IMGUI_DIR)/misc/cpp
CXXFLAGS += -g -Wall -Wformat -Iinclude
LIBS = -lcurl

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags` `pkg-config --static --libs mpv glew x11`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2 mpv`

	CXXFLAGS += `pkg-config --cflags sdl2`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------


$(BUILD_DIR)/%.o:$(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(SOURCE_DIR)/layer/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(SOURCE_DIR)/extractor/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(SOURCE_DIR)/widgets/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/misc/cpp/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
