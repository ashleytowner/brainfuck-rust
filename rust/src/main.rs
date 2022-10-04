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

fn main() {
    let mut tape = Tape::new();
    tape.set(3);
    tape.increment();
    tape.shift_right();
    tape.increment();
    tape.shift_left();
    tape.shift_left();
    tape.decrement();
    println!("Value: {}", tape.get())
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
}
