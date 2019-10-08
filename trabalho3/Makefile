CC = g++ -std=c++11
DOMAIN_FILES = main.cpp config_reader.cpp arena.cpp circle.cpp line.cpp player.cpp geometric_forms.cpp
TINYXML_FILES = tinyxml2.cpp
OUTPUT_FILE = trabalhocg
FLAGS = -lGL -lGLU -lglut -lm

all: g++
	@echo "Para executar a aplicacao, utilize um dos seguintes comandos:"
	@echo "(1) ./trabalhocg <config_path>"
	@echo "(2) make run CONFIG=<config_path>\n"

g++:
	@$(CC) $(DOMAIN_FILES) $(TINYXML_FILES) -o $(OUTPUT_FILE) $(FLAGS)

run:
	@./$(OUTPUT_FILE) $(CONFIG)

clean:
	@rm -f $(OUTPUT_FILE)
