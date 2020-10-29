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
	string usage = "Usage: bfi [-ic] file";
	string file = "";
	string program;
	bool flag_i = false;
	bool flag_c = false;
	if (argc == 1) {
		std::cin >> program;
	} else if (argc == 2) {
		file = argv[1];
	} else if (argc == 3) {
		file = argv[2];
		int index = 0;
		while (argv[1][index] != '\0') {
			switch (argv[1][index]) {
				case 'i':
					flag_i = true;
					break;
				case 'c':
					flag_c = true;
					break;
				default:
					break;
			}
			index++;
		}
	} else {
		cerr << usage << endl;
		return 1;
	}
	if (file.length() > 0) {
		ifstream infile(file);
		string line;
		while (getline(infile, line)) {
			program += line;
		}
	}
	if (flag_c) {
		string c_program =
				"#include <stdio.h>\nint main() {\nchar ptr[30000] = {0};\nint i = "
				"0;\n";
		for (int i = 0; i < program.length(); i++) {
			char instruction = program[i];
			switch (instruction) {
				case '+':
					c_program += "++ptr[i];\n";
					break;
				case '-':
					c_program += "--ptr[i];\n";
					break;
				case '<':
					c_program += "--i;\n";
					break;
				case '>':
					c_program += "++i;\n";
					break;
				case ',':
					c_program += "ptr[i]=getchar();\n";
					break;
				case '.':
					c_program += "putchar(ptr[i]);\n";
					break;
				case '[':
					c_program += "while (ptr[i]) {\n";
					break;
				case ']':
					c_program += "}\n";
					break;
				default:
					break;
			}
		}
		c_program += "return 0;\n}";
		std::cout << c_program << endl;
		return 0;
	}
	Machine machine = Machine();
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
					std::cout << (char)machine.getCell();
				} else {
					std::cout << (int)machine.getCell() << " ";
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
				programCounter++;
		}
	}
	return 0;
}