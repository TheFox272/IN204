CC = g++
CFLAGS = -Wall -Werror -std=c++20
INC = include/
LIBS = -lm -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC = src/
BUILD = build/
EXEC = main


all: $(BUILD)$(EXEC)

$(BUILD)$(EXEC): $(BUILD)main.o $(BUILD)game.o $(BUILD)display.o $(BUILD)entities.o $(BUILD)effects.o
	$(CC) $(CFLAGS) -g $^ -o $@ $(LIBS)

$(BUILD)%.o: $(SRC)%.cpp $(INC)%.hpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INC) -g -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD)
