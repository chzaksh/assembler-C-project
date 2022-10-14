/********	Assembler progrem, writen by Chanoch Zaksh and Ariel Polski *********/

#include "data.h"

/** Functions of main.c **/

int is_file(int argc);
int pre_assembler(FILE* file1, FILE* file2, char* fileName);
void assembler(char* fileName);
void reset_counters();

/** Functions of the assembler.c **/

int first_pass(FILE*);
int second_pass(FILE*);
void free_arrays();
void free_name_file();
/** Functions of the "firstPass.c" and "helpFunc.c" and "PassFunc.c" **/

char* create_end_name_of_file(char*, char*);
char* get_line(FILE* file1, char* arr_line);
char* get_next_word(char* line, char* type);
char* skip_white_space(char* line);
int check_first_word(char* token, int step);
int insert_macro(FILE* file1, char* arr_line);
int valid_macro(char* p, char* name_macro);
int check_end_line(char* p);
int check_label(char* word);
int search_command(char*);
int additional_words(char* firstOperand, char* secondOperand, int firstMethod, int secondMethod);
int valid_method(int cmd, int method, int type);
int get_method(char*);
void search_macro_in_line(char*arr_line,FILE* file1, FILE* file2);
void set_error(char* error);
void search_label(char* token, int type);
void reset_varb();
void deal_with_command(char* line,int cmd, int move);
void deal_with_data(char* line);
void deal_with_extern(char* line);
void arr_comb();
int compare_arr_extern(char* label);
int compare_arr_label(char* label);
void memory_error();

/* Functions that coding binary code word and inserting it to suitable array */
unsigned int creating_cmd_word(int cmd, int firstMethod, int secondMethod, char* firstOperand, char* secondOperand);
void code_data(char* token, int are);
void code_string(char* line);
void insert_ic_word(unsigned int word);
void insert_dc_word(unsigned int word);
void insert_1_reg(char* reg, int operandNum);
void insert_2_reg(char* reg1, char* reg2);
void insert_op(char* operand ,int method, int operandNum, int move);
void check_cells();
int is_struct(char* op);
int check_struct(char* line);

/* Functions of secondPass.c */
char *base_32_convertion(int word);
unsigned int split_bits(unsigned int word, int first, int second);
void update_arr_ic(char* operand);
int execute_files(char* fileName);
void deal_with_entry(char* line);

/* coding the addressing method with numbers*/
enum methods {IMMEDIATE, DIRECT, STRUCT, REGISTER};

