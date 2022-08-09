# Brainfuck Interpreter

This is a simple [brainfuck](https://en.wikipedia.org/wiki/Brainfuck) interpreter.

## Specifications

This brainfuck interpreter has a tape length of 65536 (2^16) cells, each containing 1 byte.

### Instructions

| Instruction | Meaning                                                                     |
| :---------: | :-------------------------------------------------------------------------- |
|      >      | Move data pointer right                                                     |
|      <      | Move data pointer left                                                      |
|      +      | Increment byte at pointer by 1                                              |
|      -      | Decrement byte at pointer b 1                                               |
|      .      | Output the byte at pointer as an ascii character                            |
|      ,      | Accept one byte of input and set the value at pointer to that input byte    |
|      [      | If byte at data pointer is zero, jump data pointer forwards to _matching_ ] |
|      ]      | If byte at data pointer is not zero, jump data pointer back to _matching_ [ |

## Commandline Args

- `-d` Display output as decimal numbers rather than ASCII characters
- `-x` Display output as hexadecimal numbers rather than ASCII characters
- `-c` transpile BF program to C, and print to stdout.
- `-m` print the tape at the end.

## Example

```bf
++++++++[>++++++++<-]>++.B<++++[>++++<-]>.R<++++[>----<-]>-.A++++++++.I+++++.N--------.F<++++[>++++<-]>-.U<++++[>----<-]>--.C++++++++.K
```
