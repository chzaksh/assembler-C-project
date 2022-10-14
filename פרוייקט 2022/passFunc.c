/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

/***
	This file holds most of the functions of the first pass and some of then used by the sacond pass as well.
***/


#include "data.h"
#include "definitions.h"
#include "global_vars.h"
#include "assembler.h"
#include <math.h>

int isError;
int second_op = FALSE;								
int firstField = FALSE;
int secondField = FALSE;
int countLabel = 0;											/*Counter of the label we found*/
int countExternLabel = 0;
int twoBits = 2;												/*Two bits to shift when coding word to an array*/
int count_execute_extern = 0;
Label* arr_labels;
Extern* arr_extern;
Extern* execute_extern;

int count_of_cells;											/*Counter of the cells of the array we used(out of 156 cells)*/
unsigned int arr_DC[MAX_CELLS];						   /*Array of the directive sentences*/
unsigned int arr_IC[MAX_CELLS];							/*Array of the commands sentences*/
unsigned int arr_final[MAX_CELLS];						/*Array of the command and directive sentences*/
int countExternLabel;										/*Counter that holds how many extern labels found*/
int countEntry;												/*Counter of the entry label we found*/
																		
/*
	Function frees all the pointers to an arrays that we saved with realloc function.
*/
void free_arrays()
{
	free_name_file();
	free(arr_labels);
	free(arr_extern);
	free(execute_extern);	
}

/*
	Function resetting all of the counters used during the progrem.
*/
void reset_counters()
{
	count_macro = 0;
	countLabel = 0;
	countExternLabel = 0;
	count_execute_extern =0;
	countExternLabel = 0;											
	countEntry = 0;													
	count_of_cells = 0;
}
																			
								
/* 
	Function that resets all the variables in tha start of every line.
*/
void reset_varb()
{
	isLabel = isData = isExtern = isString = isStruct = duplicateLabel = isEntry = FALSE;
}

/*
	Function that combining the arrays arr_IC and arr_DC.
	param move - in which pass we in, the first or the second.
*/
void arr_comb(int move)
{
	int i;
	int j;
	if(move == FIRST_PASS)
	{
		for(i=0; i < IC;i++)
			arr_final[i] = arr_IC[i];

		for(j=0; j < DC;i++, j++)
			arr_final[i] = arr_DC[j];
		 
		for(i=0; i < countLabel; i++)
		{
			if (arr_labels[i].isData)
				arr_labels[i].address += IC+100;
		}
	}
	else
	{
		for(i=0; i < IC;i++)
			arr_final[i] = arr_IC[i];
	}
}

/*
	Function that search label if we found it already or not.
	param char* token - the word we checking.
	param int type - the type of label we looking, extern label, entry label or just label.
*/
void search_label(char* token, int type)
{
	if(type == SEARCH_LABEL) 
	{
		if(countLabel == 0 && compare_arr_extern(token))						/*Search if we already found the label*/
		{
			arr_labels = (Label*)realloc(arr_labels, sizeof(Label)*(countLabel+1));
			if(!arr_labels)
				memory_error();

			arr_labels[countLabel].name = token;
			arr_labels[countLabel].address = IC+100;
			arr_labels[countLabel].isEntry = 0;
			countLabel++;
		}
		else if(compare_arr_label(token) && compare_arr_extern(token))		/*Search if we already found the label*/
		{
			arr_labels = (Label*)realloc(arr_labels, sizeof(Label)*(countLabel+1));
			
		 	if(!arr_labels)
				memory_error();
		 
			arr_labels[countLabel].name = token;
			arr_labels[countLabel].address = IC+100;
			arr_labels[countLabel].isEntry = 0;
			countLabel++;
		}	
	}
	else 
	{
		if(countExternLabel == 0 && compare_arr_label(token))					/*Search if we already found the label*/
		{
			arr_extern = (Extern*)realloc(arr_extern, sizeof(Extern)*(countExternLabel+1));
			if(!arr_extern)
				memory_error();
		
			arr_extern[countExternLabel].name = token;
			countExternLabel++;			
		}
	 	else if(compare_arr_label(token) && compare_arr_extern(token))		/*Search if we already found the label*/
	 	{
			arr_extern = (Extern*)realloc(arr_extern, sizeof(Extern)*(countExternLabel+1));
			if(!arr_extern)
				memory_error();
			
			arr_extern[countExternLabel].name = token;
			countExternLabel++;				
	 	}
	}	
}

/*
	Function that creating the first word of command.
	param cmd - the number of command between 0-15.
	param firstMethod - the type of the first operand.
	param secondMethod - the type of the second operands.
	param firstOperand - the first opernd.
	param secondOperand - the second operand.
*/
unsigned int creating_cmd_word(int cmd, int firstMethod, int secondMethod, char* firstOperand, char* secondOperand)
{
	unsigned int word = 0;
	word = (int)cmd;
	word <<= twoBits;
   
	if(firstOperand && secondOperand)
   {   
		word = word | firstMethod;
		word <<= twoBits; 												/* Leave space for second addressing method */
		word = word | secondMethod;
	}															
	else if(firstOperand)												/* If there is a singal operand (a single operand is a destination operand). */
	{
		word <<= twoBits; 
		word = word | firstMethod;
 	} 
	else if(!firstOperand && !secondOperand)
	{
		word <<= twoBits; 
	}
 	word <<= twoBits;; 													/* inserting ARE bits */
	return word;
}

/*
	Function that calculating the additional words for saving space for them, because we can't code them until the second pass.
	param firstOperand - the first operand of the command.
	param secondOperand - the second operand of the command.
	param firstMethod - the type of the first operand.
	param secondMethod - the type of the second operands.
*/
int additional_words(char* firstOperand, char* secondOperand, int firstMethod, int secondMethod)
{
  int numWords = 0;
  if(firstOperand)
  {
    if(firstMethod == STRUCT)										/*If the method is struct method*/
       numWords += 2;
		else        
		 numWords += 1;
  }
  if(secondOperand) 
  {
    if(secondMethod == STRUCT)									/*If the method is struct method*/
      numWords += 2;
    else        
		numWords += 1;
  }
  /* If both methods are register, they will share the same extra word */
  if((firstOperand && secondOperand) && (firstMethod == 3) && (secondMethod == 3)) 
      numWords--;
  return numWords;
}

/*
	Function checks if a word can be a label.
	param word - the word we cehcking.
*/
int check_label(char* word)
{
	int end = strlen(word);
	int i = 1;
	
	if(strlen(word) > 31)
		return FALSE;
	if(!isalpha(word[0]))
		return FALSE;
	while(i < end)
	{
		if(!isalnum(word[i-1]))
			return FALSE;
		i++;		
	}
	i = 0;	
	while(saveWord[i])
	{
		if(strcmp(word, saveWord[i]) == 0)
			return FALSE;
		i++;
	}
	return TRUE;
}

/*
	Function checks if a given token can be a data line = integer number.
	param token - the series of chars we check.
*/
int check_data(char* token)
{
	int i = 0;
	int end; 
	token = get_next_word(token, " ");
	end = strlen(token);

	if(token[0] == ',')
		return FALSE;
	if(token[0] == '+' || isdigit(token[0]))
	{
		if(token[0] == '+')
			i = 1;
		
		while(i < end)
		{
			if(!isdigit(token[i]))
				return FALSE;
			i++;
		}
		return TRUE;
	}
	else if(token[0] == '-')
	{	
		while(i < end)
		{
			if(!isdigit(token[1]))	
				return FALSE;
			i++;
		}
		return TRUE;
	}
	return FALSE;
}

/*
	Function checks if a given line is a valid string.
	param line - the line we checking.
*/
int check_string(char* line)
{
	int i = 1;
	int end = strlen(line);

	if(line[0] != '"')
		return FALSE;
	while(line && line[i] != '"')
		i++;
	if((i+1 == end) && line[i + 1] != '\0')
		return FALSE;
	return TRUE;
}

/*
	Function deals with extern label if found and if legal, inserting it to a linked list of an extern labels.
	param line - the line we found the extern label.
*/
void deal_with_extern(char* line)
{
	char* token;
	line = skip_white_space(line);
	token = get_next_word(line, "\n");
	token = get_next_word(token, " ");
	
	if(!check_label(token))
		set_error(ILEGAL_LABEL_EXTREN);
	else if(!check_end_line(line + strlen(token) + 1))
		set_error(ILEGAL_LINE);
	else
	 search_label(token, SEARCH_EXTERN);
}

/*
	Function deals wite lines that has a command word.
	param arr_line - the line we handaling and inserting.
	param cmd - the command we the progrem found.
	param move - a flag that points as in which stage pass the progrem are, the first or the second.
*/
void deal_with_command(char* arr_line, int cmd, int move)
{
	char* firstOperand, *secondOperand;								/*2 pointers to recive the opernds methods*/
	char* line = skip_white_space(arr_line);						/*The line we recive as a parameter and we handling*/
	unsigned int word;													/*An unsigned int that recive the binary word*/
													
	if(cmd < 4 || cmd == 6)												/*Check if the command is type of 2 operands*/
	{	
		firstOperand = get_next_word(line, ",");
		if(!isalpha(firstOperand[0]) && firstOperand[0] != '#')
			set_error(ILEGAL_OPERAND);
		else
		{
			line += strlen(firstOperand) + 1;						/*Moving the line forward after the comma*/
			line = skip_white_space(line);
			
			secondOperand = get_next_word(line, " ");
			if(!isalpha(secondOperand[0]) && secondOperand[0] != '#')
				set_error(ILEGAL_OPERAND);
			
			line += strlen(secondOperand);							/*Moving the line forward after the second opernd*/
			if(!check_end_line(line))
				set_error(ILEGAL_LINE);

			if(!isError)
			{
				int firstMethod = get_method(firstOperand);		/*Getting the type of method of the first opernd*/
				int secondMethod;
				second_op = TRUE;
				secondMethod = get_method(secondOperand);			/*Getting the type of method of the second opernd*/
				
				if(firstMethod >= 0 && secondMethod >= 0 && valid_method(cmd, firstMethod, 1) && valid_method(cmd,secondMethod,2))
				{
					if(move == FIRST_PASS)								/*If the progrem is in first pass*/
					{	
						word = creating_cmd_word(cmd, firstMethod, secondMethod, firstOperand, secondOperand);
						insert_ic_word(word);
					}
					else														/*If the progrem is in second pass*/
					 IC++;							
																				
					if(firstMethod == REGISTER && secondMethod == REGISTER)	/*If both operands are registers*/
					{
						if(move == FIRST_PASS)
							insert_2_reg(firstOperand + 1, secondOperand + 1);
						else													/*If the progrem is in second pass*/
							IC++;
				 	}					
				else							/*If the progrem is in first move and command has 2 opernds, send them to coding into binary*/
				{
					 insert_op(firstOperand , firstMethod,1, move);
					 insert_op(secondOperand, secondMethod,2, move);
				}
				check_cells();												/*Checking if the cells of the progrem is fool*/
			}	
			else
				set_error(ILEGAL_METHOD);
			}					
		}
	}
	else if(cmd > 13)														/*Check if the command is type of no operands*/	
	{
		if(!check_end_line(line))
			set_error(ILEGAL_OPERAND);
		else if(!isError)
		{
			if(move == FIRST_PASS)
			{
				word = creating_cmd_word(cmd, 0, 0, '\0', '\0');
				insert_ic_word(word);
			}
			else																/*If the progrem is in second pass*/
				IC++;
		}			
	}	
	else																		/*Check if the command is type of 1 operand*/				
	{
		firstOperand = get_next_word(line, " ");
		firstOperand = get_next_word(firstOperand, ",");
		if((!isalpha(firstOperand[0]) && firstOperand[0] != '#') || !check_end_line(line + strlen(firstOperand)))
			set_error(ILEGAL_OPERAND);	
		
		if(!isError)
		{
			int firstMethod = get_method(firstOperand);			/*Getting the type of method of the first opernd*/
			if(firstMethod >= 0 && valid_method(cmd, firstMethod, 2))
			{
				if(move == FIRST_PASS)
				{
					word = creating_cmd_word(cmd, firstMethod, 0, firstOperand, '\0');
					insert_ic_word(word);
				}
				else															/*If the progrem is in second pass*/
					IC++;	
			  insert_op(firstOperand, firstMethod,1, move);
			  check_cells();
		 	}
			else
			  set_error(ILEGAL_METHOD);				
		}
	}
}

/*
	Function checks if the addressing method is valid according the type of command.
	param cmd - the number of command.
	param method - the type of addressing method.
	param type - source or destination operand method.
*/
int valid_method(int cmd, int method, int type)
{
	int i = 0;
	if(type == 1)
	{
		for(; i < 4; i++)
		{
			if(cmd_arr[cmd].sourceAddress[i] == method)			/*Searching the addressing method according to the given command*/
				return TRUE;
		}
		return FALSE;
	}
	else
	{
		for(; i < 4; i++)
		{
			if(cmd_arr[cmd].targetAddress[i] == method)			/*Searching the addressing method according to the given command*/
				return TRUE;
		}
		return FALSE;
	}
}

/*
	Function checks if their is space left in the machin memory.
*/

void check_cells()
{
	if(count_of_cells > MAX_CELLS)			
 		set_error(CELLS_IS_FOOL);
}

/*
	Function creating a 1 word made of 2 registers.
	param reg1 - the first register.
	param reg2 - the second register.
*/
void insert_2_reg(char* reg1, char* reg2)
{
	unsigned int word = 0;

	word <<= 4;
	word |= (int) atoi(reg1);
	word <<= 4;
	word |= (int) atoi(reg2);
	word <<= twoBits;

	insert_ic_word(word);
}

/*
	Function inserting a binary word to an array of IC.
	param word - the word the function inserting.
*/
void insert_ic_word(unsigned int word)
{
	if(count_of_cells < MAX_CELLS)
	{
		arr_IC[IC] = word;
		IC++;
		count_of_cells++;
	}
	else	
		set_error(CELLS_IS_FOOL);
}

/*
	Function creating a word made from 1 register.
	param reg - register.
*/
void insert_1_reg(char* reg, int operandNum)
{	
	unsigned int word = 0;
	if(operandNum == 1)
	{
		word <<= twoBits*2;
		word |= (int)atoi(reg +1);
		word <<= twoBits*3;

		insert_ic_word(word);
	}
	else
	{
		word <<= twoBits*4;
		word |= (int)atoi(reg +1);
		word <<= twoBits;

		insert_ic_word(word);
	}
}

/*
	Function creating a binary word from operand, by method and data of operand.
	param method - the type of method.
	param operandNum - number of operand, first or second.
	param moce - the first or second pass.
*/
void insert_op(char* operand ,int method, int operandNum, int move)
{
	if(move == FIRST_PASS)									/*If the progrem is in first pass*/
	{
		if(method == IMMEDIATE)
		{
			operand++;
			code_data(operand, TRUE);
		}
		else if(method == REGISTER)
			insert_1_reg(operand, operandNum);
		
		else if(method == STRUCT)
		{
			IC++;
			if(operand[strlen(operand)-2] == '1')
			{
				insert_ic_word(4);
				count_of_cells++;
			}
			else
			{
				insert_ic_word(8);
				count_of_cells++;
			}
		}
		else														/*If the method is in direct method, handling in second pass*/						
		{
			IC++;
			count_of_cells++;
		}
	}
	else															/*If the progrem is in second pass*/
	{
		char* token;
		if(method == IMMEDIATE)
			IC++;
		else if(method == REGISTER)
			IC++;
		else if(method == STRUCT)
		{
			token = get_next_word(operand, ".");
			update_arr_ic(token);
			IC++;
		}
		else														/*If the method is in direct method*/
		{
			int i = 0;
			int externFlag = 0;
			token = get_next_word(operand, "\n");
			token = get_next_word(token, " ");

			for(; i < countExternLabel; i++)				/*Checks if the label is an Extern label in the array of extern labels*/
			{	
				if(strcmp(token, arr_extern[i].name) == 0) 
				{ 
					execute_extern = (Extern*)realloc(execute_extern,sizeof (Extern)*(count_execute_extern+1));
					if(!execute_extern)
						memory_error();
					
					execute_extern[count_execute_extern].name = token;
					execute_extern[count_execute_extern].address = IC + 100;
					count_execute_extern++;					/*Increasing the number of times an extern label has uccord in the file*/
					insert_ic_word(1);
					externFlag = TRUE;
					break;
				}
			}
			if(!externFlag)			
				update_arr_ic(token);
		}
	}
}	

/*
	Function updating the labels array and flags the entry labels.
	param line - the line of the entry label.
*/
void deal_with_entry(char* line)
{
	int i = 0;
	char* token;
	line = skip_white_space(line);
	token = get_next_word(line, "\n");
	token = get_next_word(token, " ");	

	if(!check_label(token))
		set_error(ILEGAL_LABEL_ENTRY);
	else
	{
		for(; i < countLabel;i++)
		{
			if(strcmp(token, arr_labels[i].name) == 0)					/*Search if we alradey found the label*/
			{
				arr_labels[i].isEntry = TRUE;
				countEntry++;
			}
		}
	}	
}

/*
	Function updaets the IC array with the label address after she first pass.
	param operand - the operand of the label.
	param method - the type of method of the label.
*/
void update_arr_ic(char* operand)
{
	unsigned int word = 0;
	int i;
	
	for(i=0; i < countLabel; i++)
	{
		if(strcmp(operand, arr_labels[i].name) == 0)
		{
			word = arr_labels[i].address;
			word <<= twoBits;
			word |= 2;
			insert_ic_word(word);
			break;
		}
	}
}

/*
	Function checks if the addressing method is STRUCT and if it's a valid field(1 or 2, but not higher).
	param word - the word the function checks.
*/
int method_struct(char* word)
{
   char* token = get_next_word(word, ".");

	if(check_label(token))
	{
		if(second_op)
	  		token = get_next_word(word, " ");
		token = get_next_word(word, ",");
	  
		if(token[strlen(token)-1] == 1)
			firstField = TRUE;
		if(token[strlen(token)-1] == 2)
			secondField = TRUE;
		return 2;
	}
 
 	return -1;
}

/*
	Function getting the type of method from a token.
	param token - the token the function checks.
*/
int get_method(char* token)
{
	int i = 0;
	int end;
	token = get_next_word(token, " "); 
 	end = strlen(token);

	if(token[0] == '#')
	{	
		token++;
		end--;
		if(token[0] == '+' || token[0] == '-')
		{
			token++;
			end--;			
		}
		while(i < end)
		{	
			if(!isdigit(token[i]))
				set_error(ILEGAL_OPERAND);
			i++;
		}
		return 0;
	}
	if(check_label(token))
		return 1;
	if(is_struct(token))
		return method_struct(token);
	if(strlen(token) <= 3 && token[0] == 'r' && token[1] <= '7' && token[1] >= '0')/*Check if the method register is valid*/
		return 3; 
	return -1;
}			

/*
	Function is checking if the token is a struct according to the '.' befor the number 1/2.
	param op - thr token we checking.
*/
int is_struct(char* op)
{
	op = get_next_word(op, " ");
	op = skip_white_space(op);
	if(op[strlen(op)-2] == '.')
			return TRUE;
	return FALSE;
}

/*
	Function deals with data line(data/string/struct) and coding it to DC array.
	param line - the line of data.
*/
void deal_with_data(char* line)
{
	int i = 0;
	char* op = "1";
	char* op2;
	
	if(isLabel)
	{
		arr_labels[countLabel-1].address = DC;
		arr_labels[countLabel-1].isData = TRUE;
	}
	if(isStruct)
	{
		if(!check_struct(line))
			set_error(ILEGAL_OPERAND);
		else
		{
		line = skip_white_space(line);

		op = get_next_word(line, ",");
		line += strlen(op) + 1;
		line = skip_white_space(line);
		op2 = get_next_word(line, " ");
																			
		code_data(op, FALSE);
		code_string(op2);
		}				 
	}																																																														
	else if(isString)													/*string line(alpha charecters)*/
	{
		
		line = skip_white_space(line);
		
		op = get_next_word(line, " ");
		if(!check_string(op))
			set_error(ILEGAL_OPERAND);
		else
			code_string(line);
	}
	else																	/*data line(integers)*/
	{	
		int end;
		line = get_next_word(line, "\n");
		line = skip_white_space(line);
		end = strlen(line);	
		
		while(end > 0 && !isError)									/*Coding the line of integers*/
		{
			line = skip_white_space(line);
			op = get_next_word(line, ",");
			line += strlen(op) + 1;	
			end = strlen(line);
			
			if((!check_data(op) && i < end))
				set_error(ILEGAL_OPERAND);
			code_data(op, FALSE);
		}
	}	
}

/*
	Functoin checks if struct line is valid according to the 2 fields of struct.
	param line - the line of struct the function checks.
*/
int check_struct(char* line)
{	
	char* op1;
	char* op2;
	line = skip_white_space(line);
	op1 = get_next_word(line, ",");
	line += strlen(op1)+1;
	op2 = get_next_word(line, " ");
	
	if(op2 && (check_data(op1)) && check_string(op2))
		return TRUE;
	return FALSE;
}

/*
	Functions checks and returns the number of command we found.
	param word - the word of command.
*/
int search_command(char* word)
{
	int i = 0;
	
	while(i < 16)
	{
		if(strcmp(cmd_arr[i].name, word) == 0)
			return i;
		i++;
	}
								
	set_error(ILEGAL_COMMANDE);						
	return -1;
}

/*
	Function coding a string line and send it to inserting.
	param line - the line of string.
*/
void code_string(char* line)
{
	int i = 0;
	unsigned int word = 0;
	
	line++;

	while(line[i] != '"')
	{
		word = 0;
		word <<= 10;
		word |= line[i];
		insert_dc_word(word);
		i++;
	}
	insert_dc_word(0);
}

/*
	Function inserting a data(string/struct/data) words into DC array.
	param line - the 
*/
void insert_dc_word(unsigned int word)
{
	if(count_of_cells < MAX_CELLS)
	{
		arr_DC[DC] = word;
		DC++;
		count_of_cells++;
	}
	else	
		set_error(CELLS_IS_FOOL);
}

/*
	Function coding a data line and send it to inserting.
	param token - the line of data.
	param are are - if ARE bits are needed to insert.
*/
void code_data(char* token, int are)
{
	unsigned int word = 0;
	int temp1 = atoi(token);
	int temp2 = ( int)temp1;
	if(!are || temp1 >= 0)
	{			
		word <<= 10;
		word |= temp2;
		insert_dc_word(word);
	}
	else
	{
		word <<= 8;
		word |= temp2;
		word <<= twoBits;
		word |= 0;
		insert_ic_word(word);
	}
}

/*
	Function is spliting a 10 bits word to 2 words for converting them into 32 base word.
	param first - the first bit sequence.
	param last - the last bit sequence.
*/
unsigned int split_bits(unsigned int word, int first, int last)
{
    int num;
    int length = last - first + 1; 
    int mask = (int)pow(2, length) - 1; 					/* creating a mask  */

    mask <<= first; 
    num = word & mask; 
    num >>= first; 
    return num;
}

/* 
	Function converting a word to 2 chars in 32 base.
	param num - the num the function converts. 
*/
char *base_32_convertion(int num)
{
  char *base32_array = (char *) malloc(3);
	base32_array[0] = base32[split_bits(num, 5, 9)];
 	base32_array[1] = base32[split_bits(num, 0, 4)];
 	base32_array[2] = '\0';

  return base32_array;
}

/*
	Function checks if we already found the label.
	param label - the label we checking now.
*/
int compare_arr_label(char* label)
{
	int i;
	if(countLabel == 0)
		return TRUE;
	for(i=0; i < countLabel; i++)
	{	
		if(strcmp(arr_labels[i].name, label) == 0)
		{
			set_error(DUPLICATE_LABEL);																
			duplicateLabel = TRUE;
			return FALSE;
		}
	}
	return TRUE;
}

/*
	Function checks if we already found the extern label.
	param label - the label we checking now.
*/
int compare_arr_extern(char* label)
{
	int i;
	if(countExternLabel == 0)
		return TRUE;

	for(i=0; i < countExternLabel; i++)
	{	
		if(strcmp(arr_extern[i].name, label) == 0)
		{
			set_error(DUPLICATE_LABEL);				/*printing an error command not exist*/	
			duplicateLabel = TRUE;
			return FALSE;
		}
	}
	return TRUE;
}

/*
	Function creating the ext, ent and ob files.
	param filename - the file name with the new ending.
*/
int execute_files(char* fileName)
{
	int i;
	char* file_of_end_ent;								/*Pointer to the entry file*/
	char* file_of_end_ext;								/*Pointer to the extern file*/
	char* file_of_end_ob;								/*Pointer to the object file*/

	FILE* newFile;
	
	if(countEntry)											/*Executing the entry file*/
	{
		file_of_end_ent = create_end_name_of_file(fileName, ".ent");
		if (!(newFile = fopen(file_of_end_ent, "w")))
			memory_error();
		
		rewind(newFile);
		for(i = 0; i < countLabel; i++)
		{	
			if(arr_labels[i].isEntry)
				fprintf(newFile,"%s\t%s\n",arr_labels[i].name, base_32_convertion(arr_labels[i].address));
		}
		fclose(newFile);
	}

	if(countExternLabel)									/*Executing the extern file*/
	{
		file_of_end_ext = create_end_name_of_file(fileName, ".ext");
		if (!(newFile = fopen(file_of_end_ext, "w")))
			memory_error();
		rewind(newFile);
		for(i = 0; i < count_execute_extern; i++)
			fprintf(newFile,"%s\t%s\n",execute_extern[i].name, base_32_convertion(execute_extern[i].address));
		fclose(newFile);
	}
	file_of_end_ob = create_end_name_of_file(fileName, ".ob");
	if (!(newFile = fopen(file_of_end_ob, "w")))
		memory_error();
	
	rewind(newFile);
	fprintf(newFile,"%4s\t%s\n\n",base_32_convertion(IC),base_32_convertion(DC));
	for(i = 0; i < (IC+DC); i++)						/*Executing the ob file*/
		fprintf(newFile,"%4s\t%s\n",base_32_convertion(100+i),base_32_convertion(arr_final[i]));
	
	return TRUE;
}    




