CC= gcc
CFLAGS= -W -Wall
LDFLAGS=
SRC= src
INC= include
OBJ= obj
BIN= bin

all: directories SINGLE_CONVERTER CLIENT_SERVER MULTI_CONVERTER

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

# ==========
# EXECUTABLE
# ==========
SINGLE_CONVERTER: $(OBJ)/single_converter.o $(OBJ)/converters.o
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS) 

CLIENT_SERVER: CONVERSION_CLIENT CONVERSION_SERVER

CONVERSION_CLIENT: $(OBJ)/conversion_client.o $(OBJ)/converters.o
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS) 

CONVERSION_SERVER: $(OBJ)/conversion_server.o $(OBJ)/converters.o
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS) 

MULTI_CONVERTER: $(OBJ)/multi_converter.o $(OBJ)/converters.o
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS) 

# =====
# OBJET
# =====
$(OBJ)/converters.o: $(SRC)/converters.c
	$(CC) -c $^ -o $@ $(CFLAGS) -I$(INC)

$(OBJ)/single_converter.o: $(SRC)/single_converter.c
	$(CC) -c $^ -o $@ $(CFLAGS) -I$(INC)

$(OBJ)/conversion_client.o: $(SRC)/conversion_client.c
	$(CC) -c $^ -o $@ $(CFLAGS) -I$(INC)

$(OBJ)/conversion_server.o: $(SRC)/conversion_server.c
	$(CC) -c $^ -o $@ $(CFLAGS) -I$(INC)

$(OBJ)/multi_converter.o: $(SRC)/multi_converter.c
	$(CC) -c $^ -o $@ $(CFLAGS) -I$(INC)

.PHONY: all proper clean cleanall run1 run2 run3

proper:
	rm -f $(SRC)/*~ $(INC)/*~ *~

clean: proper
	rm -f $(OBJ)/*.o $(BIN)/*

cleanall: clean
	rm -r $(OBJ) $(BIN)

run1:
	@echo "SINGLE_CONVERTER"
	${BIN}/SINGLE_CONVERTER CNY "100.0"

run2:
	@echo "CLIENT_SERVER"
	${BIN}/CONVERSION_SERVER ${BIN}/prequete ${BIN}/preponse &
	${BIN}/CONVERSION_CLIENT ${BIN}/prequete ${BIN}/preponse CNY "100.0" &

run3:
	@echo "MULTI_CONVERTER"
	${BIN}/MULTI_CONVERTER ${BIN}/prequete ${BIN}/preponse &
	${BIN}/CONVERSION_CLIENT ${BIN}/prequete ${BIN}/preponse CNY "100.0" &
