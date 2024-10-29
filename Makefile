PROGRAM = scripter

SOURCES = main.c string_helpers.c vars.c
HEADERS = main.h string_helpers.h vars.h

OBJ_FILES = $(patsubst %.c,%.o,$(SOURCES))

all: $(PROGRAM)

$(PROGRAM): $(OBJ_FILES)
	gcc -o $(PROGRAM) $^ 

$(OBJ_FILES): %.o: %.c
	gcc -c $^ -o $@