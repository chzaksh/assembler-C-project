/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

/*
	This file holds the definitions of registers, array of structs of commands, 32 base array and array that holds all 
	the saved words of the program.
*/

const char* arr_reg[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

const char* saveWord[] = {".data", ".string", ".struct", ".extern", ".entry","mov", "cmp", "add", "sub", "not", "clr", "lea",
								  "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

const char base32[32] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a',
								 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
								 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};

typedef struct
{
	char* name;
	int num_of_op;
  	int sourceAddress[4];
	int targetAddress[4];
}Cmd;

Cmd cmd_arr[] = {
						{"mov", 2, {0,1,2,3}, {1,2,3,-1}},
						{"cmp", 2, {0,1,2,3}, {0,1,2,3}},
						{"add", 2, {0,1,2,3}, {1,2,3,-1}},
						{"sub", 2, {0,1,2,3}, {1,2,3,-1}},
						{"not", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"clr", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"lea", 2, {1,2,-1,-1}, {1,2,3,-1}},
						{"inc", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"dec", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"jmp", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"bne", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"get", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"prn", 1, {-1,-1,-1,-1}, {0,1,2,3}},
						{"jsr", 1, {-1,-1,-1,-1}, {1,2,3,-1}},
						{"rts", 0, {-1,-1,-1,-1}, {-1,-1,-1,-1}},
						{"hlt", 0, {-1,-1,-1,-1}, {-1,-1,-1,-1}}
					  };



