#![allow(dead_code)]
struct Tape {
    pointer: usize,
    vec: Vec<u8>
}

impl Tape {
    fn new() -> Tape {
        let mut vec = Vec::new();
        vec.push(0);
        Tape { pointer: 0, vec }
    }

    fn extend_right(&mut self) {
        self.vec.push(0);
    }

    fn extend_left(&mut self) {
        let mut new_start = vec![0];
        new_start.append(&mut self.vec);
        self.vec = new_start;
    }

    fn shift_right(&mut self) {
        self.pointer += 1;
        if self.pointer >= self.vec.len() {
            self.extend_right();
        }
    }

    fn shift_left(&mut self) {
        if self.pointer == 0 {
            self.extend_left();
            return;
        }
        self.pointer -= 1;
    }

    fn increment(&mut self) {
        if self.vec[self.pointer] == 0xFF {
            self.vec[self.pointer] = 0;
            return;
        }
        self.vec[self.pointer] += 1;
    }

    fn decrement(&mut self) {
        if self.vec[self.pointer] == 0 {
            self.vec[self.pointer] = 0xFF;
            return;
        }
        self.vec[self.pointer] -= 1;
    }

    fn set(&mut self, value: u8) {
        self.vec[self.pointer] = value;
    }

    fn get(&self) -> u8 {
        self.vec[self.pointer]
    }

    fn char(&self) -> char {
        self.get() as char
    }
}

struct Program {
    commands: Vec<char>,
    pointer: usize,
    tape: Tape,
    out: Vec<char>
}

impl Program {
    fn new() -> Program {
        Program { commands: Vec::new(), pointer: 0, tape: Tape::new(), out: Vec::new() }
    }

    fn feed_char(&mut self, cmd: char) {
        let byte = match cmd {
            '+' | '-' | '>' | '<' | '[' | ']' | '.' | ',' => cmd,
            _ => '\0',
        };
        if byte != '\0' {
            self.commands.push(cmd);
        }
    }

    fn feed_line(&mut self, line: &str) {
        for cmd in line.chars() {
            self.feed_char(cmd);
        }
    }

    fn current_cmd(&self) -> char {
        self.commands[self.pointer]
    }

    fn locate_matching_bracket(vec: &Vec<char>, pointer: usize) -> Option<usize> {
        let mut depth = 0;
        match vec[pointer] {
            ']' => {
                for n in (0..pointer-1).rev() {
                    if vec[n] == '[' && depth == 0 {
                        return Some(n);
                    }
                    if vec[n] == ']' {
                        depth += 1;
                    }
                    if vec[n] == '[' {
                        depth -= 1;
                    }
                }
                return None;
            },
            '[' => {
                for n in pointer+1..vec.len() {
                    if vec[n] == ']' && depth == 0 {
                        return Some(n);
                    }
                    if vec[n] == '[' {
                        depth += 1;
                    }
                    if vec[n] == ']' {
                        depth -= 1;
                    }
                }
                return None;
            },
            _ => None,
        }
    }

    fn execute(&mut self) {
        if self.pointer >= self.commands.len() {
            return;
        }
        match self.current_cmd() {
            '+' => self.tape.increment(),
            '-' => self.tape.decrement(),
            '>' => self.tape.shift_right(),
            '<' => self.tape.shift_left(),
            '.' => {
                self.out.push(self.tape.char());
                println!("{}", self.tape.char());
            },
            '[' => {
                if self.tape.get() == 0 {
                    self.pointer = match Program::locate_matching_bracket(&self.commands, self.pointer) {
                        None => self.pointer,
                        Some(i) => i + 1
                    }
                } else {
                    self.pointer += 1;
                };
                self.execute();
            },
            ']' => {
                self.pointer = match Program::locate_matching_bracket(&self.commands, self.pointer) {
                    None => self.pointer,
                    Some(i) => i
                };
                self.execute();
            }
            _ => ()
        };
        self.pointer += 1;
        self.execute();
    }
}

fn main() {
    let mut program = Program::new();
    // program.feed_line("+++++[>++++++<-]>+++...");
    // program.execute();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn value_wraparound() {
        let mut tape = Tape::new();
        tape.decrement();
        assert_eq!(tape.get(), 0xFF);
        tape.set(0xFF);
        tape.increment();
        assert_eq!(tape.get(), 0);
    }

    #[test]
    fn extensions() {
        let mut tape = Tape::new();
        tape.set(5);
        tape.shift_left();
        tape.shift_left();
        assert_eq!(tape.get(), 0);
        tape.shift_right();
        tape.shift_right();
        assert_eq!(tape.get(), 5);
        tape.shift_right();
        tape.shift_right();
        assert_eq!(tape.get(), 0);
    }

    #[test]
    fn ascii_conversion() {
        let mut tape = Tape::new();
        tape.set(65);
        assert_eq!(tape.char(), 'A');
    }

    #[test]
    fn bracket_matching() {
        assert_eq!(
            Program::locate_matching_bracket(
                &vec!['+','[','+',']','-'],
                1).unwrap(),
            3
        );
        assert_eq!(
            Program::locate_matching_bracket(
                &vec!['+','[','+',']','-'],
                3).unwrap(),
            1
        );
        assert_eq!(
            Program::locate_matching_bracket(
                &vec!['+', '+', '+', '+', '+', '[', '>', '+', '+', '+', '+', '+', '+', '<', '-', ']', '+', '+', '+', '.'],
            5).unwrap(),
            15
        );
        assert_eq!(
            Program::locate_matching_bracket(
                &vec!['+', '+', '+', '+', '+', '[', '>', '+', '+', '+', '+', '+', '+', '<', '-', ']', '+', '+', '+', '.'],
            15).unwrap(),
            5
        );
    }

    #[test]
    fn hello_world() {
        let mut program = Program::new();
        program.feed_line(">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<+ +.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>- ]<+.");
        program.execute();
        assert_eq!(program.out, vec!['H','e','l','l','o',',',' ','W','o','r','l','d','!'])
    }
}
