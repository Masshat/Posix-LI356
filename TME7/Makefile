CC= gcc
CFLAGS= -W -Wall
LDFLAGS= -L $(LIB) -lficindex
SRC= src
INC= include
OBJ= obj
LIB= lib
BIN= bin
EXEC= AUTOTESTFIC

all: directories $(EXEC) 

directories: ${LIB} ${BIN}

${LIB}:
	mkdir -p ${LIB}
${BIN}:
	mkdir -p ${BIN}

# ==========
# EXECUTABLE
# ==========
$(EXEC): $(OBJ)/autotestfic.o $(LIB)/libficindex.a
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS) 

# ============
# BIBLIOTHEQUE
# ============
$(LIB)/libficindex.a: $(OBJ)/ficindex.o
	ar -rs $@ $<

# =====
# OBJET
# =====
$(OBJ)/ficindex.o: $(SRC)/ficindex.c
	$(CC) -c $^ -o $@ $(CFLAGS) -I$(INC)

.PHONY: all clean proper run

proper:
	rm -f $(SRC)/*~ $(INC)/*~ *~

clean: proper
	rm -f $(OBJ)/ficindex.o $(LIB)/* $(BIN)/*

cleanall: clean
	rm -r $(LIB) $(BIN)

run:
	$(BIN)/$(EXEC)
