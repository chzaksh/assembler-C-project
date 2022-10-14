/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

#include "data.h"
#include "definitions.h"
#include "global_vars.h"

int count_line = 0;
int isData = FALSE;
int isLabel = FALSE;
int isExtern = FALSE;
int isError;
int isEntry = FALSE;
int isString;
int isStruct;
char* newName;

/*
	Function resetting the name of the previos file that the progrem operate on.
*/
void free_name_file()
{
	free(newName);
}

/*
	Function checks if the user send a adlist file to operat on.
	param int argc - number of words the usre send.
*/
int is_file(int argc)
{
	if (argc < 2)
	{
		set_error(NO_FILE);
		return FALSE;
	}
	return TRUE;
}

/* 
	This function creates a file name by adding suitable ending.
	param file_Name - the current file name.
	param type - the type of ending we adding.
*/
char* create_end_name_of_file(char *file_Name, char* type)
{
	newName = (char *) malloc(strlen(file_Name) + MAX_EXTENSION_LENGTH);
   if(newName == NULL)
   	memory_error();

   strcpy(newName, file_Name);	 /* Copying filename with end name */

   /* Concatenating the required file extension */
	if(strcmp(type,".as") == 0)
		strcat(newName, ".as");

	if(strcmp(type,".am") == 0)
		strcat(newName, ".am");

	if(strcmp(type,".ob") == 0)
		strcat(newName, ".ob");

	if(strcmp(type,".ent") == 0)
		strcat(newName, ".ent");

	if(strcmp(type,".ext") == 0)
		strcat(newName, ".ext");
   return newName;
}

/*
	Function is getting the next kine of the file.
	param file1 - the file we reading from.
	param arr_line - the array that recives a copy of the current line.
*/
char* get_line(FILE* file1, char* arr_line)
{
	count_line++;
   if(fgets(arr_line, LINE_SIZE, file1) == NULL)
      return NULL;				/*get line from file till '\n' or 81 characters.*/
	return arr_line;
}

/*
	Function that getting the next word in line.
	param line - the line we in right now.
	param type - a parameter to operate with.
*/
char* get_next_word(char* line, char* type)
{
	char* new_arr = (char*)malloc(sizeof(char)*LINE_SIZE);  
	char* token;
	line = skip_white_space(line);
	strcpy(new_arr, line);
   token = strtok(new_arr, type);
	
	if(type[0] == ' ')
		token = strtok(token, "\t");
	return token;
}
		
/*
	Function is setting an error if found.
	param error - the error send. 
*/																	  
void set_error(char* error)
{
	isError= TRUE;
	printf("%s, line: %d\n", error, count_line);
}  

/*
	Function that checks wht kind are the first word of line.
	param token - the word we checking.
	param step - which pass we in. the first or the second.
*/
int check_first_word(char *token, int step)
{
  if(step == PRE_ASSEMBLER)
	{
		if(strcmp(token,"macro") == 0)
		{
			isMacro = TRUE;
			return TRUE;
		}
		else if(strcmp(token,"endmacro") == 0)
		{
			isMacro = FALSE;
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(strcmp(token, ".data") == 0)
	{
		isData = TRUE;
		return TRUE;
	}
	else if(strcmp(token, ".string")== 0)
	{
		isString = TRUE;
		return TRUE;
	}
	else if(strcmp(token, ".struct")== 0)
	{
		isStruct = TRUE;
		return TRUE;
	}
	else if(strcmp(token, ".extern")== 0)	
	{
		isExtern = TRUE;
		return TRUE;
	}
	else if(strcmp(token, ".entry")== 0)	
	{
		isEntry = TRUE;
		return TRUE;
	}		
	else if(token[strlen(token)-1] == ':')
	{
		token = get_next_word(token, ":");
		if(check_label(token))
		{
			if(step == FIRST_PASS)
				search_label(token, SEARCH_LABEL);
			isLabel = TRUE;
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}	

/*
	Function is skipping on white spaces.
	param line - the line we in.
*/																															   
char*skip_white_space(char* line)
{
	int i;
	for(i=0 ;i < LINE_SIZE; i++)
		if(line[i] == ' ' || line[i] == '\t')
			;
		else 
			break;
	return line+i;
}

/*
	Function is searching macro in the line.
	param ptrLine - pointer to the line.
	param file1 - the file we reading from.
	param file2 - the file we writing in.
*/
void search_macro_in_line(char* ptrLine, FILE* file1, FILE* file2)
{
	int i = 0;
	char first_word[LINE_SIZE];									/*max charcters of the name macro*/
	int machMacro = 0;												/*Flag if we found the macro's name twice*/
	ptrLine = skip_white_space(ptrLine);

	while(ptrLine[i] != ' ' && ptrLine[i] != '\t' && ptrLine[i] != '\n')
	{
		sscanf(ptrLine,"%s", first_word);
		i++;
	}

	for(i=0; i < count_macro; i++)
	{
		if((strcmp(first_word, arr_macros[count_macro-1-i].title) == 0))
		{
			fputs(arr_macros[count_macro-1-i].data, file2);
			machMacro = TRUE;
			break;
		}
	}

	if(!machMacro)
		fputs(ptrLine, file2);										/*coping the line to the new file*/
} 

/*
	Function is inserting macro to an array.
	param file1 - the file we reading from.
	param ptrLine - the macro we found.
*/
int insert_macro(FILE* file1, char* ptrLine)
{
	int i = 0;
	char name_macro[75];												/*max charcters of the name macro*/
	char* token;
	arr_macros[count_macro-1].title = (char*)malloc(sizeof(char) * 75);
	arr_macros[count_macro-1].data = (char*)malloc(sizeof(char) * LINE_SIZE);
	
	if(!arr_macros[count_macro-1].data || !arr_macros[count_macro-1].title) 
		memory_error();
	ptrLine = skip_white_space(ptrLine);
	while(ptrLine[i] != ' ' && ptrLine[i] != '\t' && ptrLine[i] != '\n')			/*coping the name of the macro into array*/
	{
		sscanf(ptrLine,"%s", name_macro);
		i++;
	}
		
	if(!valid_macro(ptrLine+i, name_macro))
		set_error(ILEGAL_MACRO);
	
	strcpy(arr_macros[count_macro-1].title, name_macro);								/*saving the title of the macro*/

	while(isMacro)																					/*saving the lines of the macro*/
	{
		ptrLine = get_line(file1, ptrLine);
		ptrLine = skip_white_space(ptrLine);
		if(!check_end_line(ptrLine))
		{
			token = get_next_word(ptrLine, " ");
			check_first_word(token, PRE_ASSEMBLER);
			i=1;
			while(isMacro)
			{
				arr_macros[count_macro-1].data = (char*)realloc(arr_macros[count_macro-1].data ,(sizeof(char) * LINE_SIZE) * i);
				
				if(!arr_macros[count_macro-1].data) 
					memory_error();
			
				strcat(arr_macros[count_macro-1].data, ptrLine);
				ptrLine = get_line(file1, ptrLine);
				ptrLine = skip_white_space(ptrLine);
			
				if(!check_end_line(ptrLine))
				{
					token = get_next_word(ptrLine, " ");
					token = get_next_word(token, "\n");
					check_first_word(token, PRE_ASSEMBLER);
				}
				i++;
			} 
		}
	}
	return TRUE;
}

/*
	Function that get the first chacerter in a word.
	param ptrLine - the line we in.
	param first_word - the word the function returns.
*/
char* get_first_charecter(char* ptrLine, char* first_word)
{
	int i = 0;
	while(ptrLine[i] != ' ' && ptrLine[i] != '\t' && ptrLine[i] != '\n')
	{
		sscanf(ptrLine,"%s", first_word);
		i++;
	}
	return first_word;
}

/*
	Function is checking if the name of the macro is taken or not.
	param ptrLine - the line we are in.
	param name_macro - the macro the program found now.
*/
int valid_macro(char* ptrLine, char* name_macro)
{
	int i =0;
	if(!check_end_line(ptrLine))
		return FALSE;
	for(; i < count_macro; i++)
	{
		if(strcmp(name_macro, arr_macros[count_macro-1-i].title) == 0)
			return FALSE;		
	}
	return TRUE;
}

/*
	Function checks if the line ended.
	param ptrLine - the line we in.
*/
int check_end_line(char* ptrLine)
{	
	ptrLine = skip_white_space(ptrLine);
	if(ptrLine[0] == '\n' || ptrLine[0] == '\0' || !ptrLine)
		return TRUE;
	return FALSE;
}

void memory_error()
{
	fprintf(stderr, "There is not enough memory for the program.");
	exit(0);
}

