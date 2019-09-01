OGL_FLAGS = -lGL -lGLU -lglut -lm
CPP_FILES = main.cpp circle.cpp
OUTPUT_FILE = output
TINYXML_DIR = $(PWD)/tinyxml

all: g++
	@$(MAKE) -C $(TINYXML_DIR) -Wno-error -w -s > /dev/null
	@echo "Done."

g++: $(CPP_FILES)
	@g++ -std=c++11 $(CPP_FILES) -o $(OUTPUT_FILE) $(OGL_FLAGS)

run:
	@./$(OUTPUT_FILE)

clean:
	@rm -f $(OUTPUT_FILE)
	@$(MAKE) -C $(TINYXML_DIR) clean -s
