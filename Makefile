# by matsuori

SRC=src/
LLS=$(SRC)libraries/
SBJ=objects/

OBJ=$(SBJ)main.o \
$(SBJ)display.o \
$(SBJ)connection.o

FIN=$(BUI)charcell
LIB=-lSDL2
BUI=build/
CC=g++ -Wall -std=c++23


$(FIN) : $(OBJ) $(LLS)libcserialport.a $(BUI)
	$(CC) $(OBJ) $(LLS)libcserialport.a -o $(FIN) $(LIB)

$(SBJ)main.o : $(SRC)main.cpp $(SBJ)
	$(CC) -I$(SRC)CSerialPort/include -c $(SRC)main.cpp -o $(SBJ)main.o

$(SBJ)display.o : $(SRC)display.cpp $(SRC)display.hpp $(SBJ)
	$(CC) -c $(SRC)display.cpp -o $(SBJ)display.o

$(SBJ)connection.o : $(SRC)connection.cpp $(SRC)connection.hpp $(SBJ) \
$(LLS)libcserialport.a
	$(CC) -I$(SRC)CSerialPort/include \
-c $(SRC)connection.cpp -o $(SBJ)connection.o

$(LLS)libcserialport.a : $(LLS) $(SRC)CSerialPort/build/
	cmake -S $(SRC)CSerialPort -B $(SRC)CSerialPort/build -DBUILD_SHARED_LIBS=OFF
	cmake --build $(SRC)CSerialPort/build
	cp $(SRC)CSerialPort/build/lib/libcserialport.a $(LLS)

$(SRC)CSerialPort/build/ : $(SRC)CSerialPort/
	mkdir $(SRC)CSerialPort/build

$(LLS) :
	mkdir $(LLS)
$(SBJ) :
	mkdir $(SBJ)

$(BUI) :
	mkdir $(BUI)


run : $(FIN)
	./$(FIN)

clean :
	rm -r $(SBJ)
	rm $(FIN)
	rm -r $(BUI)
