CC=gcc
CFLAGS=-Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

ifneq ($(SIZE), )
CFLAGS += -DSTACK_SIZE=$(SIZE)
endif

all: directories runstack

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}
${LIB}:
	mkdir -p ${LIB}

# =====
# OBJET
# =====
${OBJ}/stack_array.o: ${SRC}/stack_array.c ${INC}/stack.h
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/stack_list.o: ${SRC}/stack_list.c ${INC}/stack.h ${INC}/list.h
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/list_impl.o: ${SRC}/list_impl.c ${INC}/stack.h
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/fifo_list.o: ${SRC}/fifo_list.c ${INC}/fifo.h ${INC}/list.h
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/stack_test.o: ${SRC}/stack_test.c ${INC}/stack.h
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/fifo_test.o: ${SRC}/fifo_test.c ${INC}/stack.h
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libstack.a : ${OBJ}/stack_array.o ${OBJ}/stack_list.o ${OBJ}/list_impl.o
	ar -rs $@ $^

${LIB}/libfifo.a : ${OBJ}/fifo_list.o ${OBJ}/list_impl.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
${BIN}/stack_test: ${OBJ}/stack_test.o ${LIB}/libstack.a
	${CC} -o $@ $^

${BIN}/fifo_test: ${OBJ}/fifo_test.o ${LIB}/libfifo.a
	${CC} -o $@ $^

runstack: ${BIN}/stack_test ${BIN}/fifo_test
	@echo ""
	@echo "*************"
	@echo "Testing Stack"
	${BIN}/stack_test
	@echo "*************"
	@echo "Testing Fifo"
	${BIN}/fifo_test

.PHONY: all proper clean cleanall

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}
