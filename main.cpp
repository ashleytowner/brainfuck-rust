#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Machine.cpp"

using namespace std;

int locateMatch(string program, int index) {
	char token = program[index];
	if (token == '[') {
		int depth = 0;
		index++;
		while (index < program.length()) {
			token = program[index];
			if (token == ']' && depth == 0) {
				return index;
			} else if (token == ']' && depth > 0) {
				depth--;
			} else if (token == '[') {
				depth++;
			}
			index++;
		}
		return -1;
	} else if (token == ']') {
		int depth = 0;
		index--;
		while (index >= 0) {
			token = program[index];
			if (token == '[' && depth == 0) {
				return index;
			} else if (token == '[' && depth > 0) {
				depth--;
			} else if (token == ']') {
				depth++;
			}
			index--;
		}
		return -1;
	} else {
		return -1;
	}
}

int main(int argc, char const *argv[]) {
	string usage = "Usage: bfi [-i] file";
	string file;
	bool flag_i = false;
	if (argc == 2) {
		file = argv[1];
	} else if (argc == 3) {
		file = argv[2];
		int index = 0;
		while (argv[1][index] != '\0') {
			if (argv[1][index] == 'i') {
				flag_i = true;
			}
			index++;
		}
	} else {
		cerr << usage << endl;
		return 1;
	}
	Machine machine = Machine();
	string program;
	ifstream infile(file);
	getline(infile, program);
	int programCounter = 0;
	while (programCounter < program.length()) {
		char instruction = program[programCounter];
		switch (instruction) {
			case '+':
				machine.incrementCell();
				programCounter++;
				break;
			case '-':
				machine.decrementCell();
				programCounter++;
				break;
			case '>':
				machine.movePointerRight();
				programCounter++;
				break;
			case '<':
				machine.movePointerLeft();
				programCounter++;
				break;
			case '.':	 // Print value
				if (!flag_i) {
					cout << (char)machine.getCell();
				} else {
					cout << (int)machine.getCell() << " ";
				}
				programCounter++;
				break;
			case ',':	 // Take Input
				char c;
				std::cin >> c;
				machine.setCell(c);
				programCounter++;
				break;
			case '[':	 // Jump if Zero
				if (machine.getCell() == 0) {
					programCounter = locateMatch(program, programCounter);
				} else {
					programCounter++;
				}
				break;
			case ']':	 // Jump Unless Zero
				if (machine.getCell() != 0) {
					programCounter = locateMatch(program, programCounter);
				} else {
					programCounter++;
				}
				break;
			default:
				cerr << "Syntax Error at position: " << programCounter << endl;
				return 1;
		}
	}
	return 0;
}