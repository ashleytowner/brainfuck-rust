#ifndef CELL_H
#define CELL_H
class Cell {
	public: 
		Cell();
		Cell(bool, Cell*);
		~Cell();
		char value;
		Cell* next;
		Cell* prev;
};
#endif
