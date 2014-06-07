CC=gcc
CFLAGS=-c -o
LDFLAGS=-lrt -lpthread -o
INCFLAGS= -I$(INC)
INC=include
SRC=src
OBJ=obj
BIN=bin
DEVISE=
MONTANT=

ifneq ($(DEVISE),)
DEVISE += $(DEVISE) 
endif

ifneq ($(MONTANT),)
MONTANT += $(MONTANT)
endif

all: directories CAPITALIZE CLIENT_SERVER

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

#=======#
# OBJET #
#=======#
${OBJ}/converter.o: ${SRC}/converter.c
	gcc ${CFLAGS} $@ $^ $(INCFLAGS)

${OBJ}/capitalize.o: ${SRC}/capitalize.c
	gcc ${CFLAGS} $@ $^ $(INCFLAGS)

${OBJ}/converters_client.o: ${SRC}/converters_client.c
	gcc ${CFLAGS} $@ $^ $(INCFLAGS)

${OBJ}/multi_converter.o: ${SRC}/multi_converter.c
	gcc ${CFLAGS} $@ $^ $(INCFLAGS)

#============#
# EXECUTABLE #
#============#
CAPITALIZE: ${OBJ}/capitalize.o
	gcc ${LDFLAGS} ${BIN}/$@ $^

CONVERTERS_CLIENT: ${OBJ}/converters_client.o ${OBJ}/converter.o
	gcc ${LDFLAGS} ${BIN}/$@ $^

MULTI_CONVERTER: ${OBJ}/multi_converter.o ${OBJ}/converter.o
	gcc ${LDFLAGS} ${BIN}/$@ $^

CLIENT_SERVER : MULTI_CONVERTER CONVERTERS_CLIENT

.PHONY: all proper clean cleanall run1 run2 run3

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/*

cleanall: clean
	rm -r ${OBJ} ${BIN}

run1:
	./$(BIN)/CAPITALIZE

run2:
	./$(BIN)/CONVERTERS_CLIENT shm $(DEVISE) $(MONTANT)

run3:
	./$(BIN)/MULTI_CONVERTER shm
