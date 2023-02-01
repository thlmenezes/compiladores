SRC_PATH = src
DEP_PATH = include
BIN_PATH = obj

##### File list by type
YACC_FILES = $(wildcard $(SRC_PATH)/*.y)
FLEX_FILES = $(wildcard $(SRC_PATH)/*.l)
C_FILES = $(wildcard $(SRC_PATH)/*.c)
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(C_FILES:.c=.o)))

##### Compiler Config
CC       := gcc
CFLAGS   := -I $(DEP_PATH) -I lib
YFLAGS   := -Wyacc -v -d
##### Automatic dependency creation
# sem : pq o thales falou
DEP_FLAGS = -M -MT $@ -MT $(BIN_PATH)/$(*F).o -MP -MF $@

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
$(EXEC): $(BIN_PATH)/y.tab.c $(OBJ_FILES)
	$(CC) $< -c $(CFLAGS) -o $(<:.c=.o)
	$(CC) -o $@ $(OBJ_FILES) $(CFLAGS)

##### Generate object files for our C code
$(BIN_PATH)/%.o: $(DEP_PATH)/%.d | folders
	$(CC) $(addprefix $(SRC_PATH)/,$(notdir $(<:.d=.c))) -c $(CFLAGS) -o $@

##### Generates dependency files
$(DEP_PATH)/%.d: $(SRC_PATH)/%.c | folders
	$(CC) $(CFLAGS) $< $(DEP_FLAGS)

##### Generates object files for yacc/flex code
$(BIN_PATH)/y.tab.c: $(YACC_FILES) $(DEP_PATH)/lex.yy.c
	yacc $(YFLAGS) $< -o $@
	cp $(BIN_PATH)/y.tab.h $(DEP_PATH)/y.tab.h
	cp $(BIN_PATH)/y.tab.c $(SRC_PATH)/y.tab.c

$(DEP_PATH)/lex.yy.c: $(FLEX_FILES)
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
