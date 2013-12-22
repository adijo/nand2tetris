#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<ctype.h>

#define SYMBOL_TABLE_SIZE 10
#define MAX 50

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

typedef struct Field
{
    char mnemonic[10];
    int memory_addr;
    // 1 indicates variable, 2 indicates label.
    int type;


}Field;


struct SymbolTable
{
    
    Field f[SYMBOL_TABLE_SIZE];

};

static struct SymbolTable * symtab;
int main(int argc, char * argv[])
{
    // Check if the user has provided a filename. Only one argument allowed (aside from the compiled file)
    if(argc != 2)
    {
        printf("Please enter an input asm file. \n");
        return 1;
        
    }
    
    // Open file if it exists, else return an error.
    FILE * fp = fopen(argv[1],"r");
    if(fp == NULL)
    {
        printf("Enter a valid file. %s is not a valid file. \n", argv[1]);
        return 1;
    }
    
    //Append .hack to input file name
    
    int length = strlen(argv[1]);
    char * output_file_name = malloc(length + 5);
    for(int i = 0; i < length-3; i++)
    {
        output_file_name[i] = argv[1][i];
    
    }
    
    length = length - 3;
    output_file_name[length] = 'h';
    output_file_name[length+1] = 'a';
    output_file_name[length+2] = 'c';
    output_file_name[length+3] = 'k';
    output_file_name[length+4] = '\0';
    

    //Create o/p file

    FILE * op = fopen(output_file_name, "w");
    if(op == NULL)
    {
        printf("Could not create file %s \n", output_file_name);
        return 1;
    }
    
    // Allocate space for the symbol table
    symtab = malloc(sizeof(struct SymbolTable));
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //DEMO of struct usage.
  //  symtab->f[0].memory_addr = 5;
  // symtab->f[1].type = 2;
   
   // printf("symtab[0].memory_addr equals %d and symtab[1].type equals %d \n", symtab->f[0].memory_addr,symtab->f[1].type);
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
    
    // Iterate over lines
    char buffer[MAX];
    // PASS 1 - Fill up the symbol table.
    while(fgets(buffer, sizeof(buffer), fp))
    {
        if(isEmptyLine(buffer) != 1)
        {
             // The line is non empty. 
            
            int type = findInstructionType(buffer);
            // Type 1: A instruction - variable used.
            // Type 2: Label
            // Type 3: C instruction
            // Type 4: A instruction - variable not used.
            
            if(type == 1)
            {
                // A instruction
                processAInstructionVar(buffer);
            
            }
            
            else if(type == 2)
            {
                processLabel(buffer);
            
            }
            
            else if(type == 3)
            {
            
                // C instruction.
            }
            
            else if(type == 4)
            {
            
            }
             
        
        
        
        
        
        
        }
    }
    
   print_symbol_table();
   
   return 0;


}



int isEmptyLine(char* s)
{
    // Returns 1 if a line is empty (only spaces, no characters else returns 0.
    static const char *WS = "\t\n";
    return strspn(s,WS) == strlen(s) ? 1 : 0;
}

int findInstructionType(char * s)
{

 // return 1 if it is an A instruction (with var). 2 if C instruction. 3 if label. 4 if A instruction (without var)
 
 // handle @ instructions.
 int length = strlen(s);
 for(int i = 0; i < length; i++)
 {
    if(s[i] == '@')
    {
        return isalpha(s[i+1]) ? 1 : 4;
    }

 }
 
 bool seen_start_brace = false;
 // handle labels.
 for(int i = 0; i < length; i++)
 {
    if(seen_start_brace == false && s[i] == '(')
    {
        seen_start_brace = true;
    }
    
    else if(seen_start_brace == true && s[i] == ')')
    {
        return 2;
    }
    
    
    
    
 
 }
 
  return 3;

}

int processLabel(char * s)
{
// Remove ( and ) from the buffer

int ctr = 0;
int start_place,end_place;
bool seen_start_brace = false;
int length = strlen(s);
for(int i = 0; i < length; i++)
{

    if(seen_start_brace == true && s[i] == ')')
    {
        end_place = i;
        break;
    
    }
    else if(seen_start_brace == false && s[i] == '(')
    {
        start_place = i + 1;
        seen_start_brace = true;
    }
    else if(seen_start_brace == true && (s[i] != ')' || s[i] != '('))
    {
    
        ctr++;
    
    }
    
    else if(seen_start_brace == true && s[i] == ')')
    {
        break;
    }

}



char * label = malloc(ctr + 1);
int acc = 0;
for(int i = start_place; i < end_place; i++)
{
    label[acc++] = s[i];

}
label[acc] = '\0';
//printf("len of label %s is %d \n", label,strlen(label));

// Search for the label in the symtable
int found = searchTable(label);
if(found == 1)
{
    printf("This should not have happened. \n");
}
else if(found == 2)
{

strcpy(symtab->f[symbol_table_counter].mnemonic,label);
symtab->f[symbol_table_counter].memory_addr = pc;
symtab->f[symbol_table_counter].type = 2;
symbol_table_counter++;

}

return 0;
}

int processAInstructionVar(char * s)
{
int start_pos;

int length = strlen(s);
for(int i = 0; i < length; i++)
{
    if(s[i] == '@')
    {
        start_pos = i + 1;
        
    }
    
    

}

char * var = malloc(length - start_pos + 1);
int acc = 0;
for(int i = start_pos; i < length; i++)
{
    if(s[i] == ' ' || s[i] == '\n')
    {
        break;
    }
    var[acc++] = s[i];
}
var[acc] = '\0';



if(searchTable(var) == 2)
{
    // Insert into the table.
    strcpy(symtab->f[symbol_table_counter].mnemonic,var);
    symtab->f[symbol_table_counter].memory_addr = memory_start;
    symtab->f[symbol_table_counter].type = 1;
    symbol_table_counter++;
    memory_start++;

}
return 0;

}
int searchTable(char * s)
{

// Returns 1 if the label is found in the table.
for(int i = 0; i < symbol_table_counter; i++)
{
    if(strcmp(symtab->f[i].mnemonic, s) == 0)
    {
        return 1;
    }

}
return 2;
}

void print_symbol_table()
{
    
    for(int i = 0; i < symbol_table_counter; i++)
    {
        printf("Mnemonic: %s Memory Location: %d Type: %d \n", symtab->f[i].mnemonic, symtab->f[i].memory_addr, symtab->f[i].type);
    
    }

}
   