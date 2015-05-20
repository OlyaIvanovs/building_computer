// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

(START)
// Check if a key is pressed
@KBD
D=M
@START
D; JEQ
// Fill screen
@SCREEN
M=-1
@8191
D=-A
(FILL)
@24575
A=A+D
D=A+1
A=D
M=-1
@24575
D=D-A
@FILL
D; JLT
// Check if a key is unressed
@KBD
D=M
@START
D; JNE
// Clear screen
@SCREEN
M=0
@8191
D=-A
(CLEAN)
@24575
A=A+D
D=A+1
A=D
M=0
@24575
D=D-A
@CLEAN
D; JLT
//Infinite loop
@START
0; JMP