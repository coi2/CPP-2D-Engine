ROOT_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
BASEDIR = $(ROOT_DIR)
CC = g++
FILES = $(BASEDIR)/src/*.cpp $(BASEDIR)/src/Engine/*.cpp
OUT_EXE = CPP-2D-Engine
CPPVERSION = c++0x
DIRECTORIES = -L $(BASEDIR)/lib -I $(BASEDIR)/include
ADDDEP = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32 -lglew32
# -ld3d9
EXTRA = -Wall -Wno-write-strings

all Release Debug:
	$(CC) \
	$(FILES) \
	-o $(ROOT_DIR)/bin/Debug/$(OUT_EXE) \
	-std=$(CPPVERSION) \
	$(DIRECTORIES) \
	$(ADDDEP) \
	$(EXTRA)