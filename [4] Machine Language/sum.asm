// Sums numbers from 1 to 10.
// Assign space in RAM for the i variable. i stores the current integer to be added. Lets assign RAM[20] for this.
@20
M = 1
// Assign space in RAM for sum
@21
M = 0

(LOOP)
// Load value of i into the D register
@20
D = M
@100
D = D - A
@END
D;JGT
//Now compute sum = sum + 1

// Load sum to D
@21
D = M

// Load i to A

@20
D = D + M

@21
M = D

@20
M = M + 1
@LOOP
0;JMP 



(END)
@END
0;JMP



