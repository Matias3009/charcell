# by matsuori

SRC=src/
SBJ=objects/
OBJ=$(SBJ)main.o  $(SBJ)display.o
FIN=$(BUI)charcell
LIB=-lSDL2
BUI=build/
CC=g++ -Wall


$(FIN) : $(OBJ) $(BUI)
	$(CC) $(OBJ) -o $(FIN) $(LIB)

$(SBJ)main.o : $(SRC)main.cpp $(SBJ)
	$(CC) -c $(SRC)main.cpp -o $(SBJ)main.o

$(SBJ)display.o : $(SRC)display.cpp $(SRC)display.hpp $(SBJ)
	$(CC) -c $(SRC)display.cpp -o $(SBJ)display.o

$(SBJ) :
	mkdir $(SBJ)
$(BUI) :
	mkdir $(BUI)


run :
	./$(FIN)
clean :
	rm -r $(SBJ)
	rm $(FIN)
	rm -r $(BUI)
