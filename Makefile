SRC_PATH = src
DEP_PATH = include
BIN_PATH = obj

##### File list by type
YACC_FILE = $(wildcard $(SRC_PATH)/*.y)
FLEX_FILE = $(wildcard $(SRC_PATH)/*.l)

##### Compiler Config
CC       := gcc
CFLAGS   := -I $(DEP_PATH)
YFLAGS   := -Wyacc -v -d

##### Executable name
EXEC = parser
##### Remove directories
RMDIR = rm -rdf
##### Remove files
RM = rm -f

##### Internal commands, avoid PATH
.PHONY: folders debug clean

##### General rule
all: folders | $(EXEC)

##### Generates executable
$(EXEC): $(BIN_PATH)/y.tab.c
	$(CC) $(CFLAGS) $(OUTPUT_OPTION) $<

##### Generates object files
$(BIN_PATH)/y.tab.c: $(YACC_FILE) $(DEP_PATH)/lex.yy.c
	yacc $(YFLAGS) $< -o $@
	cp $(BIN_PATH)/y.tab.h $(DEP_PATH)/y.tab.h

$(DEP_PATH)/lex.yy.c: $(FLEX_FILE)
	flex -o $@ $<

clean:
	-$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	-$(RM) $(EXEC)

folders:
	@mkdir -p $(SRC_PATH) $(DEP_PATH) $(BIN_PATH)

help:
	@echo Available commands
	@echo - clean:    Cleans generated files
	@echo - help:     Shows help
	@echo - folders:  Generates project directories
