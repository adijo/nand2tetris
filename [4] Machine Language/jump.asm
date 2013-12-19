// Demonstration of ASM JMP instruction
// Instructions executed - 1) Sets RAM[17] to 6. If RAM[17] is equal to 6, goto instruction 111. Else goto 12.



// Set register A to 6.
@6
// Set Data register to value 6.
D = A
// Set RAM[17] = 6
@17
M = D

//If RAM[17] == 6, goto 111 else goto 12

//First load the content of RAM[17] into the D register. Substract 6 from it. 

// Set register A to 17
@17
// Set D = RAM[17]
D = M
// Subtract 6 from D
@6
D = D - A
@111
D;JEQ
@12
0;JMP



