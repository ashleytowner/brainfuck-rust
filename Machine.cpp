#include <iostream>
#include "Machine.h"
#include "Cell.h"

Machine::Machine() {
	this->currentCell = new Cell();
}

uint8_t Machine::getCell() {
	return this->currentCell->value;
}

void Machine::movePointerRight() {
	if (this->currentCell->next == nullptr) {
		this->currentCell->next = new Cell(true, this->currentCell);
	}
	this->currentCell = this->currentCell->next;
}

void Machine::movePointerLeft() {
	if (this->currentCell->prev == nullptr) {
		this->currentCell->prev = new Cell(false, this->currentCell);
	}
	this->currentCell = this->currentCell->prev;
}

void Machine::incrementCell() {
	this->currentCell->value++;
}

void Machine::decrementCell() {
	this->currentCell->value--;
}

void Machine::setCell(char val) {
	this->currentCell->value = val;
}

void Machine::printMemoryDump() {
	Cell* current = this->currentCell;
	while (current->prev != nullptr) {
		current = current->prev;
	}
	while (current != nullptr) {
		std::cout << current->value;
		current = current->next;
	}
	std::cout << std::endl;
}

Machine::~Machine() {
	Cell* current = this->currentCell;
	while (current->prev != nullptr) {
		current = current->prev;
	}
	while (current != nullptr) {
		Cell* last = current;
		current = current->next;
		delete last;
	}
}
