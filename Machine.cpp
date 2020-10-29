#ifndef MACHINE
#define MACHINE
#define TAPE_LENGTH 65536

#include <iomanip>
#include <iostream>
#include <string>

class Machine {
 private:
	uint8_t tape[TAPE_LENGTH];
	int pointer;

 public:
	Machine();
	void movePointerRight();
	void movePointerLeft();
	uint8_t getCell();
	void setCell(char);
	void incrementCell();
	void decrementCell();
	void printMemoryDump();
	~Machine();
};

Machine::Machine() {
	pointer = 0;
	for (int i = 0; i < TAPE_LENGTH; i++) {
		tape[i] = 0;
	}
}

void Machine::movePointerRight() {
	pointer = (pointer + 1);
	if (pointer > TAPE_LENGTH) {
		throw "Out of Bounds";
	}
}

void Machine::movePointerLeft() {
	pointer = (pointer - 1);
	if (pointer < 0) {
		throw "Out of Bounds";
	}
}

uint8_t Machine::getCell() { return tape[pointer]; }

void Machine::setCell(char value) { tape[pointer] = (uint8_t)value; }

void Machine::incrementCell() { tape[pointer]++; }

void Machine::decrementCell() { tape[pointer]--; }

void Machine::printMemoryDump() {
	for (int i = 0; i < TAPE_LENGTH; i++) {
		printf("\n%04x: %02x", i, (int)tape[i]);
		// std::cout << i << ": " << std::hex << (int)tape[i] << std::endl;
	}
	std::cout << std::endl;
}

Machine::~Machine() {}

#endif