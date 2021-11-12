#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


const int Label_name_size = 20;
const int labels_size = 16;
const int max_pointer = 8;
const int log_size = 2000;
const int max_lexemm_length = 100;
const int data_size = 8;
const int byte_size = 8;
int IsCommandRam  = 0;

typedef struct mark 
                {
                int pointer;
                char* name;
                } mark_t;
                
class Assemb 
            {
            char* Input;
            int ri;
            FILE* log_file; 
            unsigned line_counter; 
            char* lexemm; 
            unsigned buf_size; 
            int assembling_level;
            mark_t* labels;
            public:
                char* Output;
                int wi;
                Assemb(char* Input_file_name, FILE* CMD_ENG);
                ~Assemb();
                void Assembling();
                void Push_handling();
                void Pop_handling(int IsGraphics);
                void Get_lexemm();
                void Jump_handling();
                void Label_handling();
                void Error_report(const char* message);
                void Write_number_like_chars(double number);
                void Crypting_name_of_register();
                void Putting_number_into_machine_code();
                void RAM_handling(int first_cmd, int sec_cmd);
            
            };


void* my_calloc (FILE* log_file, int size_of_elem,  int size);


enum Commands
    {
    CMD_PUSH     = 1,
    CMD_ADD      = 2,
    CMD_MUL      = 3,
    CMD_SUB      = 4,
    CMD_DIV      = 5,
    CMD_MOD      = 6,
    CMD_PRINTF   = 7,
    CMD_SCANF    = 8,
    CMD_PUSH_R   = 9,
    CMD_EXIT     = 11,
    CMD_POP_R    = 12,
    CMD_JMP      = 13,
    CMD_JMPB     = 14,
    CMD_JMPA     = 15,
    CMD_JMPE     = 16,
    CMD_JBE      = 17,
    CMD_JAE      = 18,
    CMD_JNE      = 19,
    CMD_CALL     = 20,
    CMD_RET      = 21,
    CMD_DEBUG    = 22,
    CMD_divD     = 29,
    CMD_SQRT     = 30,
    CMD_GRAPHICS = 31,

    CMD_PUSH_RAM_REG = 23,
    CMD_POP_RAM_REG  = 24,
    CMD_PUSH_RAM_NUM = 25,
    CMD_POP_RAM_NUM  = 26,
    CMD_POP_GRAM_NUM  = 27,
    CMD_POP_GRAM_REG  = 28,
    
    CMD_RAX    = 0,
    CMD_RBX    = 1,
    CMD_RCX    = 2,
    CMD_RDX    = 3,
    CMD_RSI    = 4
    };