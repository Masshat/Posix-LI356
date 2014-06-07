CC= gcc
CFLAGS= -W -Wall
SRC= src
OBJ= obj
BIN= bin

all: directories EX1_1 EX1_2 EX1_3 EX1_4 EX1_5 VOODOO EX3 EX4 EX5 ACCOUNTANT SPY

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

# =====
# OBJET
# =====

$(OBJ)/init-sig.1.o: $(SRC)/init-sig.1.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/init-sig.2.o: $(SRC)/init-sig.2.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/init-sig.3.o: $(SRC)/init-sig.3.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/init-sig.4.o: $(SRC)/init-sig.4.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/init-sig.5.o: $(SRC)/init-sig.5.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/voodoo.o: $(SRC)/voodoo.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/pending-signals.o: $(SRC)/pending-signals.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/pending-signals-2.o: $(SRC)/pending-signals-2.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/pending-signals-3.o: $(SRC)/pending-signals-3.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/accountant.o: $(SRC)/accountant.c
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ)/spy.o: $(SRC)/spy.c
	$(CC) -c $^ -o $@ $(CFLAGS)

# ==========
# EXECUTABLE
# ==========

EX1_1: $(OBJ)/init-sig.1.o
	$(CC) -o $(BIN)/$@ $^

EX1_2: $(OBJ)/init-sig.2.o
	$(CC) -o $(BIN)/$@ $^

EX1_3: $(OBJ)/init-sig.3.o
	$(CC) -o $(BIN)/$@ $^

EX1_4: $(OBJ)/init-sig.4.o
	$(CC) -o $(BIN)/$@ $^

EX1_5: $(OBJ)/init-sig.5.o
	$(CC) -o $(BIN)/$@ $^

VOODOO: $(OBJ)/voodoo.o
	$(CC) -o $(BIN)/$@ $^

EX3: $(OBJ)/pending-signals.o
	$(CC) -o $(BIN)/$@ $^

EX4: $(OBJ)/pending-signals-2.o
	$(CC) -o $(BIN)/$@ $^

EX5: $(OBJ)/pending-signals-3.o
	$(CC) -o $(BIN)/$@ $^

ACCOUNTANT: $(OBJ)/accountant.o
	$(CC) -o $(BIN)/$@ $^

SPY: $(OBJ)/spy.o
	$(CC) -o $(BIN)/$@ $^

.PHONY: all clean proper

proper:
	rm -f $(SRC)/*~ include/*~ SCRIPTS/*~
	rm -f *~

clean: proper
	rm -f $(OBJ)/*
	rm -f $(BIN)/*

cleanall: clean
	rm -r $(OBJ) $(BIN)
