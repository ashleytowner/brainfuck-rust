#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Machine.h"
#include "TapeLength.h"

using namespace std;

/// Locate a corresponding [ or ]
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
    string err = "No matching ']' found for '[' at ";
    err += to_string(index);
    throw err;
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
    string err = "No matching '[' found for ']' at ";
    err += to_string(index);
    throw err;
  } else {
    return -1;
  }
}

int main(int argc, char const* argv[]) {
  string usage = "Usage: bfi [-icm] file";
  string file = "";
  /// The text of the Brainfuck program.
  string program;
  bool flag_d = false;
  bool flag_b = false;
  bool flag_x = false;
  bool flag_c = false;
  bool flag_m = false;
  /* If no arguments are supplied,
     take BF program from stdin */
  if (argc == 1) {
    std::cin >> program;
  } else if (argc == 2) {	 // If filename supplied, take program from file.
    file = argv[1];
  } else if (argc == 3) {	 // ^ and if additional flags are supplied, note them.
    file = argv[2];
    int index = 0;
    while (argv[1][index] != '\0') {
      switch (argv[1][index]) {
        case 'd':
          flag_d = true;
          break;
        case 'x':
          flag_x = true;
          break;
        case 'b':
          flag_b = true;
          break;
        case 'c':
          flag_c = true;
          break;
        case 'm':
          flag_m = true;
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
      if (line[0] != '#' || line[1] != '!') {
        program += line;
      }
    }
  }
  // Transpile the program to C if the -c flag is enabled.
  if (flag_c) {
    string c_program = "#include <stdio.h>\nint main() {\nchar ptr[" +
      to_string(TAPE_LENGTH) +
      "] = {0};\nint i = "
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
      case '+':	 // Increment data at pointer
        machine.incrementCell();
        programCounter++;
        break;
      case '-':	 // Decrement data at pointer
        machine.decrementCell();
        programCounter++;
        break;
      case '>':	 // Move pointer right
        try {
          machine.movePointerRight();
        } catch (char const* e) {
          std::cerr << "Error: " << e << std::endl;
          return 1;
        }
        programCounter++;
        break;
      case '<':	 // Move pointer left
        try {
          machine.movePointerLeft();
        } catch (char const* e) {
          std::cerr << "Error: " << e << std::endl;
          return 1;
        }
        programCounter++;
        break;
      case '.':	 // Print value
        // if (!flag_d) {
        // 	std::cout << (char)machine.getCell();
        // }
        //  else {
        // 	std::cout << (int)machine.getCell() << " ";
        // }
        if (flag_d) {
          printf("%03d ", (int)machine.getCell());
        } else if (flag_x) {
          printf("%02x ", (int)machine.getCell());
        } else if (flag_b) {
          // printf("%08c ", (int)machine.getCell());
        } else {
          printf("%c", (int)machine.getCell());
        }
        programCounter++;
        break;
      case ',':	 // Take Input
        char c;
        c = getchar();
        machine.setCell(c);
        programCounter++;
        break;
      case '[':	 // Jump if Zero
        if (machine.getCell() == 0) {
          try {
            programCounter = locateMatch(program, programCounter);
          } catch (string e) {
            std::cerr << "Error: " << e << std::endl;
            return 1;
          }
        } else {
          programCounter++;
        }
        break;
      case ']':	 // Jump Unless Zero
        if (machine.getCell() != 0) {
          try {
            programCounter = locateMatch(program, programCounter);
          } catch (string e) {
            std::cerr << "Error: " << e << std::endl;
            return 1;
          }
        } else {
          programCounter++;
        }
        break;
      default:
        programCounter++;
    }
  }
  if (flag_m) {
    machine.printMemoryDump();
  }
  return 0;
}
