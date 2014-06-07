oCC=gcc
CFLAGS=-Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: directories ${BIN}/CREENPROCESS ${BIN}/CREEPROCESSWAIT ${BIN}/ARBRECREATION ${BIN}/DECODER ${BIN}/CONVERTERS

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

# =====
# OBJET
# =====
${OBJ}/creeNProcess.o: ${SRC}/creeNProcess.c
	${CC} -c -o $@ $< $(CFLAGS)

${OBJ}/creeProcessWait.o: ${SRC}/creeProcessWait.c
	${CC} -c -o $@ $< $(CFLAGS)

${OBJ}/arbreCreation.o: ${SRC}/arbreCreation.c
	${CC} -c -o $@ $< $(CFLAGS)

${OBJ}/decoder.o: ${SRC}/decoder.c
	${CC} -c -o $@ $< $(CFLAGS) -I$(INC)

${OBJ}/converters.o: ${SRC}/converters.c
	${CC} -c -o $@ $< $(CFLAGS) -I$(INC)

${OBJ}/mainConverters.o: ${SRC}/mainConverters.c
	${CC} -c -o $@ $< $(CFLAGS) -I$(INC)

# ==========
# EXECUTABLE
# ==========
${BIN}/CREENPROCESS: ${OBJ}/creeNProcess.o
	${CC} -o $@ $^

${BIN}/CREEPROCESSWAIT: ${OBJ}/creeProcessWait.o
	${CC} -o $@ $^

${BIN}/ARBRECREATION: ${OBJ}/arbreCreation.o
	${CC} -o $@ $^

${BIN}/DECODER: ${OBJ}/decoder.o
	${CC} -o $@ $^

${BIN}/CONVERTERS: ${OBJ}/converters.o ${OBJ}/mainConverters.o
	${CC} -o $@ $^

.PHONY: all proper clean cleanall

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN}
