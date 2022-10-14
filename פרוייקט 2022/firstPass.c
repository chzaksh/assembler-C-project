/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

#include "data.h"
#include "definitions.h"
#include "global_vars.h"


int isLabel;																			
int isData;																				
int isExtern;																			
int isEntry;																			
int isString;																			
int isStruct;																				
int isError = FALSE;																		
int IC;																						
int DC;																						
int duplicateLabel;																
int cmd;									/*Holds the number of the command(0-15)*/	
						
Label* arr_labels;					/*The pointer to an array of labels*/
Extern* arr_extern;					/*The pointer to an array of extern labels*/
Extern* execute_extern;				/*The pointer to an array of extern labels that used in the program and to print them*/

/*
	Function first_pass save labels, codes command line and directive.
	param FILE* file_am - the file we read now.
*/
int first_pass(FILE* file_am)
{
	int i;
	char* ptrLine;
	char arr_line[LINE_SIZE];
	count_line = IC = DC = 0;

	arr_labels = (Label*) malloc(sizeof(Label));
	arr_extern = (Extern*) malloc(sizeof(Extern));
	execute_extern = (Extern*)malloc(sizeof (Extern));
	if(!arr_extern || !arr_labels || !execute_extern)
		memory_error();

	rewind(file_am);
	while((ptrLine = get_line(file_am, arr_line)) != NULL)		/*Read 1 line from file and starting the first pass*/	
	{
		char* token;
		i = 0;
		ptrLine = skip_white_space(ptrLine);	
		reset_varb();															/*Resetting the variables befor the first pass*/															
			
		if(ptrLine[i] == ';' || ptrLine[i] == '\n')
			continue;
		
		ptrLine = get_next_word(ptrLine, "\n");
		token = get_next_word(ptrLine, " ");
		if(check_first_word(token, FIRST_PASS))
		{
			ptrLine = skip_white_space(ptrLine);
			ptrLine += strlen(token);
			token = get_next_word(ptrLine , " ");
			ptrLine = skip_white_space(ptrLine);
		}
		if(!isLabel && !isData && !isExtern && !isEntry)
		{
			if((cmd = search_command(token)) != -1)							/*   search the command in command tabel*/
				deal_with_command(ptrLine + strlen(token), cmd, FIRST_PASS);
		}
		else if(isLabel)
		{	
			if(duplicateLabel)														/*a flag if a label apears twice.*/ 
				continue;
			check_first_word(token,  FIRST_PASS);
			
			if(isData || isString || isStruct)
				deal_with_data(ptrLine + strlen(token));
			else if((cmd = search_command(token)) != -1)						/*search the command in command tabel*/
				deal_with_command(ptrLine + strlen(token), cmd, FIRST_PASS);	
		}
		else if(isExtern)
			deal_with_extern(ptrLine);					
		else if(isEntry)
			continue;			
		else if(isData || isString || isStruct)
			deal_with_data(ptrLine + strlen(token));		
	}
		
	if(isError)
	{
		printf("*************** The transition is failed ********************** \n");
		return FALSE;
	}
	
	arr_comb(FIRST_PASS);									/*combining the arr_DC and arr_IC to 1 array*/
	return TRUE;

}


