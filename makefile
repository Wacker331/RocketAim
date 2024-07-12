SOURCES = main.c
TARGET = nuke.exe
LIBS = 
FLAGS = -g
.PHONY: all clear debug

all: nuke.exe

nuke.exe: main.c
	gcc $(SOURCES) -o $(TARGET) $(LIBS)

debug: main.c
	gcc $(SOURCES) -o $(TARGET) $(LIBS) $(FLAGS)

clear:
	rm -f $(TARGET)