#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<ctype.h>

#define CMP_TABLE_A_ZERO_SIZE 18
#define JMP_TABLE_SIZE 7
#define CMP_TABLE_A_NOT_ZERO_SIZE 10
#define SYMBOL_TABLE_SIZE 10
#define MAX 50
#define DEST_TABLE_SIZE 7

// Static variables.
static int pc = 0;
static int memory_start = 16;
static int symbol_table_counter = 0;

// Function prototypes
int isEmptyLine(char*);
int findInstructionType(char*);
int processAInstructionVar(char*);
int processLabel(char*);
//char* processCInstruction(char*);
int searchTable(char*);
void print_symbol_table();
void processASecondPass(int);
int getMemoryAddress(char*);
int* to_binary(int);
int strip_at(char*); // Used to process type 4 instructions.
int findCType(char*);
int aBitSet(char*);

typedef struct Field
{
    char mnemonic[10];
    int memory_addr;
    // 1 indicates variable, 2 indicates label.
    int type;


} Field;


struct SymbolTable
{

    Field f[SYMBOL_TABLE_SIZE];

};


struct CompTableAZero
{
    char operation[4];
    char opcode[7];

};

struct CompTableANotZero
{
    char operation[4];
    char opcode[7];

};

struct destTable
{
    char operation[3];
    char opcode[4];

};

struct jumpTable
{
    char ops[6];
    char opc[3];
};

static struct SymbolTable* symtab;


int main(int argc, char* argv[])
{
    // Check if the user has provided a filename. Only one argument allowed (aside from the compiled file)
    if (argc != 2)
    {
        printf("Please enter an input asm file. \n");
        return 1;

    }

    // Open file if it exists, else return an error.
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Enter a valid file. %s is not a valid file. \n", argv[1]);
        return 1;
    }

    //Append .hack to input file name

    int length = strlen(argv[1]);
    char* output_file_name = malloc(length + 5);
    for (int i = 0; i < length - 3; i++)
    {
        output_file_name[i] = argv[1][i];

    }

    length = length - 3;
    output_file_name[length] = 'h';
    output_file_name[length + 1] = 'a';
    output_file_name[length + 2] = 'c';
    output_file_name[length + 3] = 'k';
    output_file_name[length + 4] = '\0';


    //Create o/p file

    FILE* op = fopen(output_file_name, "w");
    if (op == NULL)
    {
        printf("Could not create file %s \n", output_file_name);
        return 1;
    }

    // Allocate space for the symbol table
    symtab = malloc(sizeof(struct SymbolTable));
    static struct CompTableANotZero cnotzero[CMP_TABLE_A_NOT_ZERO_SIZE] = {{"M", "110000"}, {"!M", "110001"}, {"-M", "110011"}, {"M+1", "110111"}, {"M-1", "110010"}, {"D+M", "000010"}, {"D-M", "010011"}, {"M-D", "000111"}, {"D&M", "000000"}, {"D|A", "010101"}};
    static struct CompTableAZero czero[CMP_TABLE_A_ZERO_SIZE] = {{"0", "101010"}, {"1", "111111"}, {"-1", "111010"}, {"D", "001100"}, {"A", "110000"}, {"!D", "001101"}, {"!A", "110001"}, {"-D", "001111"}, {"-A", "110011"}, {"D+1", "011111"}, {"A+1", "110111"}, {"D-1", "001110"}, {"A-1", "110010"}, {"D+A", "000010"}, {"D-A", "010011"}, {"A-D", "000111"}, {"D&A", "000000"}, {"D|A", "010101"}};
    static struct destTable dest[DEST_TABLE_SIZE] = {{"M", "001"}, {"D", "010"}, {"MD", "011"}, {"A", "100"}, {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}};
    static struct jumpTable jmp[JMP_TABLE_SIZE] = {{"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //DEMO of struct usage.
    //  symtab->f[0].memory_addr = 5;
    // symtab->f[1].type = 2;

    // printf("symtab[0].memory_addr equals %d and symtab[1].type equals %d \n", symtab->f[0].memory_addr,symtab->f[1].type);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Iterate over lines
    char buffer[MAX];
    // PASS 1 - Fill up the symbol table.
    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (isEmptyLine(buffer) != 1)
        {
            // The line is non empty.

            int type = findInstructionType(buffer);
            // Type 1: A instruction - variable used.
            // Type 2: Label
            // Type 3: C instruction
            // Type 4: A instruction - variable not used.

            if (type == 1)
            {
                // A instruction
                processAInstructionVar(buffer);

            }

            else if (type == 2)
            {
                processLabel(buffer);

            }

            // Don't need to do anything for instructions of type 3,4.
            if (type != 2)
            {
                pc++;

            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    fclose(fp);
    FILE* gp = fopen(argv[1], "r");
    if (gp == NULL)
    {
        printf("Could not open file. \n");
        return 1;
    }
    print_symbol_table();
    //PASS 2 Begins.
    pc = 0;
    while (fgets(buffer, sizeof(buffer), gp))
    {


        if (isEmptyLine(buffer) != -1)
        {

            int type = findInstructionType(buffer);
            printf("%d \n", type);
            if (type == 1)
            {
                // Could be a label like @LOOP. If so handle it TODO.

                int start_pos;

                int length = strlen(buffer);
                for (int i = 0; i < length; i++)
                {
                    if (buffer[i] == '@')
                    {
                        start_pos = i + 1;

                    }



                }

                char* var = malloc(length - start_pos + 1);
                int acc = 0;
                for (int i = start_pos; i < length; i++)
                {
                    if (buffer[i] == ' ' || buffer[i] == '\n')
                    {
                        break;
                    }
                    var[acc++] = buffer[i];
                }
                var[acc] = '\0';
                int mem = getMemoryAddress(var);
                int* code = to_binary(mem);

                for (int i = 0; i < 16; i++)
                {
                    printf("%d", code[i]);
                    fprintf(op, "%d", code[i]);

                }
                printf("\n");
                fprintf(op, "\n");




            }

            else if (type == 4)
            {
                // Strip of @ in the beginning.
                int* code = to_binary(strip_at(buffer));
                for (int i = 0; i < 16; i++)
                {
                    printf("%d", code[i]);
                    fprintf(op, "%d", code[i]);

                }
                printf("\n");
                fprintf(op, "\n");


            }


            else if (type == 3)
            {
                // Process C type instruction.
                char* code = malloc(16);
                code[0] = '1';
                code[1] = '1';
                code[2] = '1';
                code[3] = '0';
                // Two types of C instructions, one of time dest = comp and the other comp;jump Find which one.
                int t = findCType(buffer);
                if (t == 1)
                {
                    // Is of type comp;jmp comp can be either D or 0.
                    int d_pos;
                    int length = strlen(buffer);
                    for (int i = 0; i < length; i++)
                    {
                        if (buffer[i] == 'D' || buffer[i] == '0')
                        {
                            d_pos = i;
                            break;
                        }
                    }
                    if (buffer[d_pos] == 'D')
                    {
                        code[4] = '0';

                        code[5] = '0';

                        code[6] = '1';

                        code[7] = '1';

                        code[8] = '0';

                        code[9] = '0';
                    }
                    else if (buffer[d_pos] == '0')
                    {
                        code[4] = '1';

                        code[5] = '0';

                        code[6] = '1';

                        code[7] = '0';

                        code[8] = '1';

                        code[9] = '0';


                    }
                    code[10] = '0';
                    code[11] = '0';
                    code[12] = '0';

                    // Now handle the jump part.
                    char jmps[3];
                    int acc = 0;
                    for (int i = d_pos + 2; i < d_pos + 5; i++)
                    {
                        jmps[acc++] = buffer[i];
                    }
                    acc = 13;
                    for (int i = 0; i < JMP_TABLE_SIZE; i++)
                    {
                        if (strcmp(jmps, jmp[i].ops) == 0)
                        {


                            for (int j = 0; j < 3; j++)
                            {
                                code[acc++] = jmp[i].opc[j];

                            }
                            break;
                        }

                    }
                    printf("Final code is %s \n", code);
                    fputs(code, op);
                    fputs("\n", op);
                }

                else
                {

                    // Is of type dest = comp. In this case, find if M is present at comp position. If yes, a = 1.
                    int is_a_bit_set = aBitSet(buffer);

                    if (is_a_bit_set == 1)
                    {

                        // A bit is set. Use CompTableANotZero table.
                        code[3] = '1';
                        int eq_at;
                        int length = strlen(buffer);
                        int dist_to_eq = 0;
                        for (int i = 0; i < length; i++)
                        {
                            if (buffer[i] == '=')
                            {
                                eq_at = i;
                                break;
                            }
                            dist_to_eq++;


                        }
                        char* desti = malloc(dist_to_eq);
                        int acc = 0;
                        for (int i = 0; i < eq_at; i++)
                        {
                            desti[acc++] = buffer[i];
                        }
                        acc = 0;
                        char* comp = malloc(length - dist_to_eq + 1);
                        for (int i = eq_at + 1; i < length - 1; i++)
                        {
                            comp[acc++] = buffer[i];
                        }
                        acc = 4;
                        for (int i = 0; i < CMP_TABLE_A_NOT_ZERO_SIZE; i++)
                        {
                            printf("Comparing %s and %s \n", comp, cnotzero[i].operation);
                            if (strcmp(comp, cnotzero[i].operation) == 0)
                            {
                                for (int j = 0; j < strlen(cnotzero[i].opcode); j++)
                                {
                                    code[acc++] = cnotzero[i].opcode[j];
                                }
                                break;
                            }


                        }
                        printf("%s \n", code);
                        for (int i = 0; i < DEST_TABLE_SIZE; i++)
                        {
                            printf("Comparing %s and %s \n", desti, dest[i].operation);

                            if (strcmp(desti, dest[i].operation) == 0)
                            {
                                for (int j = 0; j < strlen(dest[i].opcode); j++)
                                {
                                    code[acc++] = dest[i].opcode[j];
                                }
                                break;
                            }


                        }
                        for (int k = 0; k < 3; k++)
                        {
                            code[acc++] = '0';
                        }
                        fputs(code, op);
                        fputs("\n", op);

                    }

                    else
                    {
                        // A bit is not set.   Use CompTableAZero table.


                        code[3] = '0';
                        int eq_at;
                        int length = strlen(buffer);
                        int dist_to_eq = 0;
                        for (int i = 0; i < length; i++)
                        {
                            if (buffer[i] == '=')
                            {
                                eq_at = i;
                                break;
                            }
                            dist_to_eq++;


                        }
                        char* desti = malloc(dist_to_eq);
                        int acc = 0;
                        for (int i = 0; i < eq_at; i++)
                        {
                            desti[acc++] = buffer[i];
                        }
                        acc = 0;
                        char* comp = malloc(length - dist_to_eq + 1);
                        for (int i = eq_at + 1; i < length - 1; i++)
                        {
                            comp[acc++] = buffer[i];
                        }
                        acc = 4;
                        for (int i = 0; i < CMP_TABLE_A_ZERO_SIZE; i++)
                        {
                            printf("Comparing %s and %s \n", comp, czero[i].operation);
                            if (strcmp(comp, czero[i].operation) == 0)
                            {
                                for (int j = 0; j < strlen(czero[i].opcode); j++)
                                {
                                    code[acc++] = czero[i].opcode[j];
                                }
                                break;
                            }


                        }
                        printf("%s \n", code);
                        for (int i = 0; i < DEST_TABLE_SIZE; i++)
                        {
                            printf("Comparing %s and %s \n", desti, dest[i].operation);

                            if (strcmp(desti, dest[i].operation) == 0)
                            {
                                for (int j = 0; j < strlen(dest[i].opcode); j++)
                                {
                                    code[acc++] = dest[i].opcode[j];
                                }
                                break;
                            }


                        }
                        for (int k = 0; k < 3; k++)
                        {
                            code[acc++] = '0';
                        }
                        fputs(code, op);
                        fputs("\n", op);





                    }

                }

            }




            // End of processing for the second pass.
        }



    }
    fclose(gp);
    fclose(op);
    free(symtab);
    return 0;


}



int isEmptyLine(char* s)
{
    // Returns 1 if a line is empty (only spaces, no characters else returns 0.
    static const char* WS = "\t\n";
    return strspn(s, WS) == strlen(s) ? 1 : 0;
}

int findInstructionType(char* s)
{

// return 1 if it is an A instruction (with var). 3 if C instruction. 2 if label. 4 if A instruction (without var)

// handle @ instructions.
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (s[i] == '@')
        {
            return isalpha(s[i + 1]) ? 1 : 4;
        }

    }

    bool seen_start_brace = false;
// handle labels.
    for (int i = 0; i < length; i++)
    {
        if (seen_start_brace == false && s[i] == '(')
        {
            seen_start_brace = true;
        }

        else if (seen_start_brace == true && s[i] == ')')
        {
            return 2;
        }





    }

    return 3;

}

int processLabel(char* s)
{
// Remove ( and ) from the buffer

    int ctr = 0;
    int start_place, end_place;
    bool seen_start_brace = false;
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {

        if (seen_start_brace == true && s[i] == ')')
        {
            end_place = i;
            break;

        }
        else if (seen_start_brace == false && s[i] == '(')
        {
            start_place = i + 1;
            seen_start_brace = true;
        }
        else if (seen_start_brace == true && (s[i] != ')' || s[i] != '('))
        {

            ctr++;

        }

        else if (seen_start_brace == true && s[i] == ')')
        {
            break;
        }

    }



    char* label = malloc(ctr + 1);
    int acc = 0;
    for (int i = start_place; i < end_place; i++)
    {
        label[acc++] = s[i];

    }
    label[acc] = '\0';
//printf("len of label %s is %d \n", label,strlen(label));

// Search for the label in the symtable
    int found = searchTable(label);
    if (found == 1)
    {
        printf("This should not have happened. \n");
    }
    else if (found == 2)
    {

        strcpy(symtab->f[symbol_table_counter].mnemonic, label);
        symtab->f[symbol_table_counter].memory_addr = pc;
        symtab->f[symbol_table_counter].type = 2;
        symbol_table_counter++;

    }

    return 0;
}

int processAInstructionVar(char* s)
{
    int start_pos;

    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (s[i] == '@')
        {
            start_pos = i + 1;

        }



    }

    char* var = malloc(length - start_pos + 1);
    int acc = 0;
    for (int i = start_pos; i < length; i++)
    {
        if (s[i] == ' ' || s[i] == '\n')
        {
            break;
        }
        var[acc++] = s[i];
    }
    var[acc] = '\0';



    if (searchTable(var) == 2)
    {
        // Insert into the table.
        strcpy(symtab->f[symbol_table_counter].mnemonic, var);
        symtab->f[symbol_table_counter].memory_addr = memory_start;
        symtab->f[symbol_table_counter].type = 1;
        symbol_table_counter++;
        memory_start++;

    }
    return 0;

}
int searchTable(char* s)
{

// Returns 1 if the label is found in the table.
    for (int i = 0; i < symbol_table_counter; i++)
    {
        if (strcmp(symtab->f[i].mnemonic, s) == 0)
        {
            return 1;
        }

    }
    return 2;
}

void print_symbol_table()
{

    for (int i = 0; i < symbol_table_counter; i++)
    {
        printf("Mnemonic: %s Memory Location: %d Type: %d \n", symtab->f[i].mnemonic, symtab->f[i].memory_addr, symtab->f[i].type);

    }

}

int getMemoryAddress(char* s)
{
    // Returns the corressponding memory_addr for mnemonic from the symbol table

    for (int i = 0; i < symbol_table_counter; i++)
    {
        if (strcmp(s, symtab->f[i].mnemonic) == 0)
        {
            return symtab->f[i].memory_addr;

        }

    }
    return -1;
}





int* to_binary(int a)
{

    // Converts a decimal number to binary (For use only for A instruction) STRICTLY
    int* bin = malloc(16);

    for (int i = 0; i < 16; i++)
    {
        bin[i] = 0;

    }

    int acc = 15;
    while (a != 0)
    {
        bin[acc--] = a % 2;
        a = a / 2;
    }

    return bin;
}

int strip_at(char* buffer)
{

    int start_pos;

    int length = strlen(buffer);
    for (int i = 0; i < length; i++)
    {
        if (buffer[i] == '@')
        {
            start_pos = i + 1;

        }



    }

    char* var = malloc(length - start_pos + 1);
    int acc = 0;
    for (int i = start_pos; i < length; i++)
    {
        if (buffer[i] == ' ' || buffer[i] == '\n')
        {
            break;
        }
        var[acc++] = buffer[i];
    }
    var[acc] = '\0';
    int ret = atoi(var);
    free(var);
    return ret;
}

int findCType(char* s)
{
    int length = strlen(s);
    // Returns 1 if ; is present. Else returns 2.
    for (int i = 0; i < length; i++)
    {
        if (s[i] == ';')
        {
            return 1;
        }

    }
    return 2;
}

int aBitSet(char* s)
{
// returns 1 if a bit is set. 0 if it isn't.
// To do that, detect if M is present in dest=comp;jmp (comp) position. If so, a = 1.
    int eq;
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {

        if (s[i] == '=')
        {
            eq = i;
            break;
        }
    }

    return s[eq + 1] == 'M' ? 1 : 0;

}
