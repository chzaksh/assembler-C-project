/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

extern int count_line;					/*counter of the lines we read*/
extern int isMacro;						/*Flag that signes us if we find macro*/
extern int count_macro;					/*counter how many macros found*/
extern int isLabel;						/*Flag if label is found*/
extern int isData;						/*Flag if data is found*/
extern int isString;						/*Flag if the data label is string*/
extern int isStruct;						/*Flag if the data label is struct*/
extern int isExtern;						/*Flag if the label is extern*/
extern int isEntry;						/*Flag if the label is internal*/
extern int IC;								/*counter of the command word we found*/					
extern int DC;								/*counter of the directive word we found*/
extern int isError;						/*Flag if there is an error*/	
extern int duplicateLabel;				/*Flag if we found a label twice*/

/*
	Struct that holds a label, his address, and a flag if its data or entry labal,
	and pointer to that holds array os structs.
*/
typedef struct
{
	char* name;
	int address;
	int isData;
	int isEntry;
}Label;

extern Label* arr_labels;

typedef struct {
	char* title;
	char* data;
} Macro;
extern Macro* arr_macros;				/*Array of macros that the program found*/

typedef struct
{
	char* name;
	int address;
}Extern;										/*Struct of an extern label*/

extern Extern* execute_extern;		/*Array of the addresses of the extern labels the progrem found and printing them.*/						

