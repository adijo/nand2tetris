#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

#define SYMBOL_TABLE_SIZE 10
#define MAX 50
typedef struct Field
{
    char * mnemonic;
    int memory_addr;
    // 1 indicates variable, 2 indicates label.
    int type;


}Field;


struct SymbolTable
{
    
    Field f[SYMBOL_TABLE_SIZE];

};

struct SymbolTable * symtab;
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
  

}   
