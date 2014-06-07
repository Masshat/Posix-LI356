CC= gcc
CFLAGS= -W -Wall
LDFLAGS= -L $(LIB) -lrt -lmyqueue #-lpthread
INCFLAGS= -I$(INC)
INC=include
LIB=lib
SRC=src
OBJ=obj
BIN=bin
LOG=log

all: directories TEST1_myqueue TEST2_myqueue

directories: $(LIB) $(OBJ) $(BIN) $(LOG)

$(LIB):
	mkdir -p $(LIB)

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

$(LOG):
	mkdir -p $(LOG)

#=======#
# OBJET #
#=======#
$(OBJ)/myqueue.o: $(SRC)/myqueue.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)

$(OBJ)/test1_myqueue.o: $(SRC)/test1_myqueue.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)

$(OBJ)/test2_myqueue.o: $(SRC)/test2_myqueue.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)

#==============#
# BIBLIOTHEQUE #
#==============#
$(LIB)/libmyqueue.a: $(OBJ)/myqueue.o
	ar -rs $@ $^

#============#
# EXECUTABLE #
#============#
TEST1_myqueue: $(OBJ)/test1_myqueue.o $(LIB)/libmyqueue.a
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS)

TEST2_myqueue: $(OBJ)/test2_myqueue.o $(LIB)/libmyqueue.a
	$(CC) -o $(BIN)/$@ $^ $(LDFLAGS)

.PHONY: all clean proper cleanall run1 run2

proper:
	rm -f $(SRC)/*~ $(INC)/*~ $(LOG)/*~ *~

clean:proper
	rm -f $(OBJ)/* $(BIN)/* $(LIB)/* $(LOG)/*

cleanall:clean
	rm -r $(OBJ) $(LIB) $(BIN) $(LOG)

run1:
	$(BIN)/TEST1_myqueue 2> $(LOG)/err_log_test1

run2:
	$(BIN)/TEST2_myqueue 2> $(LOG)/err_log_test2
