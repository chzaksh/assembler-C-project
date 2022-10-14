/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

/*** Pre assembler handels macros saving and replace them in their place. just like "define" in c language. ***/

#include "data.h"
#include "definitions.h"
#include "global_vars.h"


int isMacro = 0;									
int count_macro = 0;							
Macro* arr_macros;

/*
	Function "preAssembler" holds all the activities regarding the pre processor.
	param file1 - the source text.
	param file2 - the destination text.
	param fileName - name of the file we handaling now.
*/

int pre_assembler(FILE* file1, FILE* file2, char* fileName)
{
	int i;
  char arr_line[LINE_SIZE];										/*array that will hold the current line for inserting to the destination file*/
	char* ptrLine = NULL;											/*pointer to the current line*/
	char* file_of_end_am;											/*pointer that holds the name of the new file*/
	
	arr_macros = (Macro*)malloc(sizeof(Macro));				/*array of the saved macros*/
	if(!arr_macros)
		memory_error();

	if(fgetc(file1) == EOF)													
	{
		fprintf(stderr, "The file %s is empty.\n", fileName);
		exit(0);
	}
	
	rewind(file1);

	file_of_end_am = create_end_name_of_file(fileName, ".am");	/*The name of the file is recived without the ending ".am".*/

 	if (!(file2 = fopen(file_of_end_am, "a")))
	{
		fprintf(stderr,"There a problem with the open in file %s \n", fileName);
		exit(0);
	}

	printf("*************** Create file %s.am process start **************** \n", fileName);
	
	rewind(file2);	
	while((ptrLine = get_line(file1, arr_line)) != NULL)			/*Read 1 line from file and doing the pre processor*/
	{
		char* token = get_next_word(arr_line, " ");					/*token - holde the next word*/
		ptrLine = skip_white_space(arr_line);
		if(check_first_word(token, PRE_ASSEMBLER))
		{
			count_macro++;
			arr_macros = (Macro*)realloc(arr_macros, sizeof(Macro) * (count_macro + 1));

			if(!insert_macro(file1, ptrLine + 5))						/*if the macro saving ddin't secceed return FALSE*/
			{
				fclose(file2);
				return FALSE;
			}
		}
		else if(count_macro > 0)											/*if we already found macros, check if we found the current macro */
			search_macro_in_line(arr_line,file1, file2);
		else
			fputs(arr_line, file2);											/*copy the line to new file*/        
	}
	
	for(i = 0; i <	count_macro; i++)										/*Freeing the pointer that were created using malloc/realloc function inside the macros array*/
	{
		free(arr_macros[i].title);
		free(arr_macros[i].data);
	}

	free(arr_macros);															/*Freeing the pointer to the macro array*/
	fclose(file2);
	
	if(isError)
		return FALSE;
	return TRUE;
   
}

