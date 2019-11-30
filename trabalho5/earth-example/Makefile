CC = g++ -std=c++11

DOMAIN_FILES = main.cpp imageloader.cpp

OUTPUT_FILE = trabalhocg
FLAGS = -lGL -lGLU -lglut -lm

all: g++
	@echo "> Compiled <"

g++:
	@$(CC) $(DOMAIN_FILES) -o $(OUTPUT_FILE) $(FLAGS)

clean:
	@rm -f $(OUTPUT_FILE)
