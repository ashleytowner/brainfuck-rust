#ifndef MACHINE
#define MACHINE
#define TAPE_LENGTH 2048

class Machine {
 private:
	uint8_t tape[TAPE_LENGTH];
	int pointer;

 public:
	Machine();
	void movePointerRight();
	void movePointerLeft();
	uint8_t getCell();
	void setCell(int);
	void incrementCell();
	void decrementCell();
	~Machine();
};

Machine::Machine() {
	pointer = 0;
	for (int i = 0; i < TAPE_LENGTH; i++) {
		tape[i] = 0;
	}
}

void Machine::movePointerRight() { pointer = (pointer + 1) % TAPE_LENGTH; }

void Machine::movePointerLeft() { pointer = (pointer - 1) % TAPE_LENGTH; }

uint8_t Machine::getCell() { return tape[pointer]; }

void Machine::setCell(int value) { tape[pointer] = value; }

void Machine::incrementCell() { tape[pointer]++; }

void Machine::decrementCell() { tape[pointer]--; }

Machine::~Machine() {}

#endif