/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

#include "data.h"
#include "definitions.h"
#include "global_vars.h"


int isLabel;
int isData;
int isExtern;
int IC;
int isEntry;
int isString;
int isStruct;


int second_pass(FILE* file_am)
{
	int i;
	char* ptrLine; 
	char arr_line[LINE_SIZE];
	count_line = IC = i = 0;

	 
	rewind(file_am);
	while((ptrLine = get_line(file_am, arr_line)) != NULL)	/*Read 1 line from file and starting the first pass*/	
	{
		char* token;
		ptrLine = skip_white_space(ptrLine);	
		reset_varb();

		if(ptrLine[i] == ';' || ptrLine[i] == '\n')
			continue;
		
		ptrLine = get_next_word(ptrLine, "\n");	
		token = get_next_word(ptrLine, " ");
		if(check_first_word(token, SECOND_PASS))
		{
			ptrLine = skip_white_space(ptrLine);
			ptrLine += strlen(token);
			token = get_next_word(ptrLine , " ");
			ptrLine = skip_white_space(ptrLine);
		}
		if(isData || isString || isStruct || isExtern)
			continue;
		else if(isLabel)
		{
			check_first_word(token, SECOND_PASS);
			if(isData || isString || isStruct)
				continue;
			else 			
				deal_with_command(ptrLine + strlen(token), search_command(token), SECOND_PASS);	
		}
		else if(isEntry)
			deal_with_entry(ptrLine);
		else
			deal_with_command(ptrLine + strlen(token), search_command(token), SECOND_PASS);
	}

	arr_comb(SECOND_PASS);							/*Combining the 2 array, IC and DC*/
	if(isError)
	{
		printf("*************** The transition is failed ********************** \n");
		return FALSE;
	}	
	return TRUE;			
}		
	
