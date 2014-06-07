CC= gcc
CFLAGS= -W -Wall
SRC= src
OBJ= obj
BIN= bin

all: directories MYWAIT MYSLEEP SYNCHRO3 CONTROL  

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

# =====
# OBJET
# =====

$(OBJ)/mywait.o: $(SRC)/mywait.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/mysleep.o: $(SRC)/mysleep.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/synchro3.o: $(SRC)/synchro3.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/control.o: $(SRC)/control.c
	$(CC) -c $^ -o $@ $(CFLAGS)

# ==========
# EXECUTABLE
# ==========

MYWAIT: $(OBJ)/mywait.o
	$(CC) -o $(BIN)/$@ $^

MYSLEEP: $(OBJ)/mysleep.o
	$(CC) -o $(BIN)/$@ $^

SYNCHRO3: $(OBJ)/synchro3.o
	$(CC) -o $(BIN)/$@ $^

CONTROL: $(OBJ)/control.o
	$(CC) -o $(BIN)/$@ $^

.PHONY: all proper clean cleanall

proper:
	rm -f $(SRC)/*~
	rm -f *~

clean: proper
	rm -f $(OBJ)/*
	rm -f $(BIN)/*

cleanall: clean
	rm -r $(OBJ) $(BIN)
