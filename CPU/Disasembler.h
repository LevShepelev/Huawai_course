#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

const int data_size = 8;
const int labels_size = 20;
const int label_name_size = 5;

class Disasm
    {
    struct mark {int pointer_; char name_[label_name_size];};
    char* Input_;
    unsigned long long  OutputSize_;
    int ri_;
    int wi_;
    int labels_top_;
    FILE* log_file_;
    mark labels_[labels_size];
    public:
    char* Output_;
    unsigned long long  InputSize_;
    
    Disasm (char* file_name);
    ~Disasm();
    void Writing_word(char* cmd_name);
    void Putchar(char a);
    void Arg_func_handling();
    void Reg_func_handling();
    void RAM_REG_func_handling();
    void RAM_NUM_func_handling();
    void jump_handling();
    void Putting_labels();
    void Disasembling();
    void Filling_disasm_file();

    };


void* my_calloc (int size_of_elem, int size);




enum Commands
    {
    CMD_PUSH    = 1,
    CMD_ADD    = 2,
    CMD_MUL    = 3,
    CMD_SUB    = 4,
    CMD_DIV    = 5,
    CMD_MOD    = 6,
    CMD_PRINTF  = 7,
    CMD_SCANF  = 8,
    CMD_PUSH_R = 9,
    CMD_EXIT   = 11,
    CMD_POP_R  = 12,
    CMD_JMP    = 13,
    CMD_JMPB   = 14,
    CMD_JMPA   = 15,
    CMD_JMPE   = 16,
    CMD_JBE    = 17,
    CMD_JAE    = 18,
    CMD_JNE    = 19,
    CMD_CALL   = 20,
    CMD_RET    = 21,
    CMD_DEBUG  = 22,
    CMD_divD   = 27,
    CMD_SQRT   = 28,

    CMD_PUSH_RAM_REG = 23,
    CMD_POP_RAM_REG  = 24,
    CMD_PUSH_RAM_NUM = 25,
    CMD_POP_RAM_NUM  = 26,
    
    CMD_RAX    = 0,
    CMD_RBX    = 1,
    CMD_RCX    = 2,
    CMD_RDX    = 3,
    CMD_RSI    = 4
    };