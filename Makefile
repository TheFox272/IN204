CC = g++
CFLAGS = -Wall -Werror
INC = include/
LIBS = -lm -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/
BUILD = build/
EXEC = main


all: $(BUILD)$(EXEC)

$(BUILD)$(EXEC): $(BUILD)main.o $(BUILD)car.o
	$(CC) $(CFLAGS) -g $^ -o $@ $(LIBS)

$(BUILD)main.o: $(SRC)main.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INC) -g -c $< -o $@

$(BUILD)car.o: $(SRC)car.cpp
	$(CC) $(CFLAGS) -I $(INC) -g -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD)