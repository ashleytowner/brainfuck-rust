#include "Cell.h"

Cell::Cell() {
	this->value = 0;
	this->next = nullptr;
	this->prev = nullptr;
}

Cell::Cell(bool setPrev, Cell* ptr) {
	this->value = 0;
	if (setPrev) {
		this->prev = ptr;
		this->next = nullptr;
		return;
	}
	this->prev = nullptr;
	this->next = ptr;
}

Cell::~Cell() {
	return;
}
