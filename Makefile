CC = g++ -std=c++11
DOMAIN_FILES = main.cpp circle.cpp cursor.cpp color.cpp screen.cpp
TINYXML_FOLDER = tinyxml
TINYXML_FILES = $(TINYXML_FOLDER)/tinyxml.cpp 			\
				$(TINYXML_FOLDER)/tinyxmlparser.cpp 	\
				$(TINYXML_FOLDER)/tinyxmlerror.cpp 		\
				$(TINYXML_FOLDER)/tinystr.cpp
OUTPUT_FILE = trab1
FLAGS = -lGL -lGLU -lglut -lm

all: g++
	@echo "Done."

g++:
	@$(CC) $(DOMAIN_FILES) $(TINYXML_FILES) -o $(OUTPUT_FILE) $(FLAGS)

run:
	@./$(OUTPUT_FILE) $(CONFIG)

clean:
	@rm -f $(OUTPUT_FILE)
