/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

/**	This program function as an Assebmbler to a uniqe base 32 machine code.
*** The progrem recives a text and interpentet it by macros, labels, commands and directives****/

#include "data.h"
#include "definitions.h"
#include "global_vars.h"


/*
	Function "main" holds all the activities of the program.
	param "argc" - the number of parameters the user send to the program.
	param "argv" - the name of the parameters send.
*/
int main(int argc, char* argv[])
{
	int i;
   char* fileName;						 						/*Recives the file to operat on*/
	char* file_of_end_as;			 							/*Pointer that holds the name of the file after adding the ending ".as".*/
	FILE* file1, *file2 = NULL;								/* file1:for reading, file2:for writing.*/
	
	if (!(is_file(argc)))										/*Check if the user send files to operate on files*/
		exit(0);
		
	for (i = 1; i < argc; i++)
	{
		fileName = argv[i];
		file_of_end_as = create_end_name_of_file(fileName, ".as"); 
		
		if (!(file1 = fopen(file_of_end_as, "r")))			/*If the file with ending ".as" exists. */
		{
			fprintf(stderr, "There a problem with the file %s\n", file_of_end_as);
			continue;													/*Pass to the next file*/
		}
		reset_counters();
		if(!pre_assembler(file1, file2, fileName))			/*Pre processor action and file creation ending with ".am".*/
		{
			char* del_file = strcat(fileName, ".am");
			remove(del_file);
			printf("*************** The transition is failed ********************** \n");
			continue;													/*Pass to the next file*/
		}
	
		fclose(file1);													/* file1 closing in assembler.c */
		printf("*************** Assembler process start *********************** \n");
		
		assembler(fileName);											/*after pre processor ends, start assembel the text according the base 32 machine*/
	}
	return 0;
}
