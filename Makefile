CC = g++
CFLAGS = -Wall -Werror -std=c++20
INC = include/
LIBS = -lm -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/
BUILD = build/
EXEC = main


all: $(BUILD)$(EXEC)

$(BUILD)$(EXEC): $(BUILD)main.o $(BUILD)game.o $(BUILD)display.o
	$(CC) $(CFLAGS) -g $^ -o $@ $(LIBS)

$(BUILD)main.o: $(SRC)main.cpp $(INC)main.hpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INC) -g -c $< -o $@

$(BUILD)game.o: $(SRC)game.cpp $(INC)game.hpp
	$(CC) $(CFLAGS) -I $(INC) -g -c $< -o $@

$(BUILD)display.o: $(SRC)display.cpp $(INC)display.hpp
	$(CC) $(CFLAGS) -I $(INC) -g -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD)