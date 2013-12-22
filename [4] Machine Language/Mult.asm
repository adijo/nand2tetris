// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm
// Student name: Aditya Joshi (1adityajoshi@gmail.com)

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[3], respectively.)

// Put your code here.

// Assign memory in RAM to store the sum

//Enter the first number to be multiplied here. Currently set to 6.
@6
D = A
@0
M = D


//Enter the second number to be multiplied here. Currently set to 7.
@7
D = A
@1
M = D


@0
D = A

@201
M = 1

@0
D = M
@200
M = D

(LOOP)
// If RAM[201] - RAM[1] < 0 exit

@201
D = M
@1
D = D - M
@END
D;JGE

@0
D = M
@200
M = D + M
@200
D = M
@2
M = D
@201
M = M + 1
@LOOP
0;JMP




(END)
@END


0;JMP



