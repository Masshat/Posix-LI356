CC= gcc
CFLAGS= -W -Wall
SRC= src
OBJ= obj
BIN= bin

all: directories PASSAGEDONNEE FORKEXEC COMPIL EX4 SPY

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

# =====
# OBJET
# =====

$(OBJ)/passageDonnee.o: $(SRC)/passageDonnee.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/fork-exec.o: $(SRC)/fork-exec.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/compil.o: $(SRC)/compil.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/ex4.o: $(SRC)/ex4.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/spy.o: $(SRC)/spy.c
	$(CC) -c $^ -o $@ $(CFLAGS)

# ===========
# EXECUTABLES
# ===========

PASSAGEDONNEE: $(OBJ)/passageDonnee.o
	$(CC) -o $(BIN)/$@ $^  

FORKEXEC: $(OBJ)/fork-exec.o
	$(CC) -o $(BIN)/$@ $^

COMPIL: $(OBJ)/compil.o
	$(CC) -o $(BIN)/$@ $^

EX4: $(OBJ)/ex4.o
	$(CC) -o $(BIN)/$@ $^

SPY: $(OBJ)/spy.o
	$(CC) -o $(BIN)/$@ $^

.PHONY: all proper clean cleanall runCompil

proper:
	rm -f $(SRC)/*~ *~

clean: proper
	rm -f $(OBJ)/* $(BIN)/*
	rm -f FICHIER_TEST/*.o FICHIER_TEST/affiche

cleanall: clean
	rm -rf ${OBJ} ${BIN}

runCompil:
	cd FICHIER_TEST;./../bin/COMPIL affiche1.c affiche2.c affiche3.c affiche4.c affiche-main.c;cd -
