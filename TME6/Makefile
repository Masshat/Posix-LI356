CC= gcc
CFLAGS= -W -Wall
SRC= src
OBJ= obj
BIN= bin

all: directories LISTE-REP SETFILESTATS FILEFUSION FINDWORD EX5 

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

# =====
# OBJET
# =====

$(OBJ)/liste-rep.o: $(SRC)/liste-rep.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/setfilestats.o: $(SRC)/setfilestats.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/filefusion.o: $(SRC)/filefusion.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/findword.o: $(SRC)/findword.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/ex5.o: $(SRC)/ex5.c
	$(CC) -c $^ -o $@ $(CFLAGS)

# ==========
# EXECUTABLE
# ==========

LISTE-REP: $(OBJ)/liste-rep.o
	$(CC) -o $(BIN)/$@ $^

SETFILESTATS: $(OBJ)/setfilestats.o
	$(CC) -o $(BIN)/$@ $^

FILEFUSION: $(OBJ)/filefusion.o
	$(CC) -o $(BIN)/$@ $^

FINDWORD: $(OBJ)/findword.o
	$(CC) -o $(BIN)/$@ $^

EX5: $(OBJ)/ex5.o
	$(CC) -o $(BIN)/$@ $^

.PHONY: all proper clean cleanall

proper:
	rm -f $(SRC)/*~
	rm -f *~ fichier.out

clean: proper
	rm -f $(OBJ)/* $(BIN)/*

cleanall: clean
	rm -r $(OBJ) $(BIN)
