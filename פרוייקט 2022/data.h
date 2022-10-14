/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

/***This file holds defintions of finals for the progrem***/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/***Flags in which stage of the progrem we in***/

#define PRE_ASSEMBLER 1
#define FIRST_PASS 2
#define SECOND_PASS 3
#define FIRST_MOVE 1
#define SEARCH_LABEL 0
#define SEARCH_EXTERN 1

#define TRUE 1
#define FALSE 0
#define ERROR 0

/***Definitions of the progrem***/

#define LINE_SIZE 81									/*Max charceters in line*/
#define MAX_EXTENSION_LENGTH 50					/*MAX charecters of the file name*/
#define MAX_CELLS 156								/*Max celles in the memory of the machine*/

/***Defintions of errors***/

#define NO_FILE "NO_EXECUTABLE_FILE_RECIVED"
#define OPERATE_DOES_NOT_EXIST "OPERATE_DOES_NOT_EXIST"
#define DUPLICATE_LABEL "REDFINE_LABEL_THAT_ALREADY_EXIST"
#define ILEGAL_OPERAND "ILEGAL_OPERAND"
#define ADDRESS_ERROR "ADDRESS_ERROR"
#define ILEGAL_METHOD "ILEGAL_METHOD"
#define CELLS_IS_FOOL "CELLS_OF_THE_MACHINE_IS_FOOL"
#define ILEGAL_COMMANDE "ILEGAL_COMMANDE_COMMAND_NOT_EXIST"
#define ILEGAL_LABEL_ENTRY "ILEGAL_ENTRY_LABEL"
#define ILEGAL_LINE "ILEGAL_LINE"
#define ILEGAL_LABEL_EXTREN "ILEGAL_EXTREN_LABEL"
#define ILEGAL_MACRO "ILEGAL_MACRO"


 
