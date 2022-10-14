/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

#include "data.h"
#include "definitions.h"

void assembler(char* fileName)
{
	
	char* file_of_end_am;
	
	FILE* file_am = NULL;																	/* file1 for reading, file2 for writing.*/
	file_of_end_am = create_end_name_of_file(fileName,".am");

	if (!(file_am = fopen(file_of_end_am, "r")))										/* if file with end name ".as" exists. */
		fprintf(stderr, "There a problem with the file %s\n", file_of_end_am);
	else 
	{
		if(first_pass(file_am))
		{
			printf("*************** First pass secsess **************************** \n");
			printf("*************** Start scond pass   **************************** \n");
				if(second_pass(file_am))
				{
					free_name_file();
					printf("*************** Second pass secsess *************************** \n");
					execute_files(fileName);
					free_arrays();
					printf("*************** The transition is Completed ******************* \n");
				}
				else
					free_arrays();				
		}
		else
			free_arrays();
	}
}
