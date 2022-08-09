#include "TapeLength.h"
#include "Cell.h"

#ifndef MACHINE
#define MACHINE

#include <iomanip>
#include <iostream>
#include <string>


class Machine {
 private:
	uint8_t tape[TAPE_LENGTH];
	int pointer;
	int highestMemoryAccessed;
	Cell* currentCell;

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
#endif
