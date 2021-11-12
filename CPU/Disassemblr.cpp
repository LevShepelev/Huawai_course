#include "Disasembler.h"

void* my_calloc (int size_of_elem, int size)
    {
    char* mem = (char*) calloc (size, size_of_elem);
    if (mem == NULL) 
        {
        printf("Calloc error\n");
        exit (1);
        }
    return mem;
    }



void* my_realloc (void* ptrmem, unsigned long long numb_of_elements, unsigned elem_size)
    {
    void* temp = (void*) realloc (ptrmem, numb_of_elements * elem_size);
    if (temp == NULL)
        {
        printf("memory reallocation problem\n");
        exit (1);
        }
    else 
        return temp;
    }



Disasm::Disasm (char* file_name)
    {
    struct stat statistica;
    int stat_error = stat (file_name, &statistica);
    assert(stat_error == 0);
    FILE* Machine_prog = fopen(file_name, "r");
    Input_  = (char*) my_calloc (statistica.st_size + 1, sizeof(char));
    Output_ = (char*) my_calloc (statistica.st_size + 1, sizeof(char));
    fread ((void*) Input_, sizeof(char), statistica.st_size, Machine_prog);
    fclose(Machine_prog);
    ri_ = 0;
    wi_ = 0;
    labels_top_ = 0;
    OutputSize_ = statistica.st_size;
    InputSize_  = statistica.st_size;
    for (int s = 0; s < labels_size; s++)
        labels_[s].pointer_ = -1;
    }



Disasm::~Disasm()
    {
    free (Input_);
    free (Output_);
    }


void Disasm::Writing_word(char* cmd_name)
    {
    printf("%s\n", cmd_name);
    if (OutputSize_ - wi_ <= 20) 
        {
        Output_ = (char*) my_realloc(Output_, 2 * OutputSize_, sizeof(char));
        OutputSize_ = 2 * OutputSize_;
        }

    for (int k = 0; k < strlen(cmd_name); k++, wi_++)
        Output_[wi_] = cmd_name[k];
    Output_[wi_] = ' ';
    wi_++;
    }



void Disasm::Putchar(char a)
    {
    Output_[wi_] = a;
    wi_++;
    }


void Disasm::Arg_func_handling()
    {
    ri_++;
    sprintf((Output_ + wi_), "%lf\n", *(double*)(Input_ + ri_));
    ri_ += data_size - 1;
    wi_ += data_size;
    }



void Disasm::Reg_func_handling()
    {
    ri_++;
    switch (Input_[ri_])
        {
        case CMD_RAX:
            Writing_word("rax");
            break;
        case CMD_RBX:
            Writing_word("rbx");
            break;
        case CMD_RCX:
            Writing_word("rcx");
            break;
        case CMD_RDX:
            Writing_word("rdx");
            break;
        case CMD_RSI:
            Writing_word("rsi");
            break;
        default:
            printf("reading error, ID = %d\n", ri_);
            exit(1);
            break;
        } 
    wi_++;
    }



void Disasm::RAM_REG_func_handling()
    {
    Putchar('[');
    Reg_func_handling();
    Putchar(']');
    }



void Disasm::RAM_NUM_func_handling()
    {
    Putchar('[');
    Arg_func_handling();
    Putchar(']');
    }



void Disasm::jump_handling()
    {
    ri_++;
    int jmp_ID = (int)*(double*)(Input_ + ri_);
    for (int s = 0; s < labels_size; s++)
        {
        if (labels_[s].pointer_ == jmp_ID)
            {
            printf("name = %s", labels_[s].name_);
            Writing_word(labels_[s].name_);
            ri_ += data_size - 1;
            return;
            }
        if (labels_[s].pointer_ == -1)
            {
            labels_top_++;
            labels_[s].pointer_ = jmp_ID;
            labels_[s].name_[0] = 'L';
            labels_[s].name_[1] = s + '0';
            labels_[s].name_[2] = ':';
            labels_[s].name_[3] = '\0';
            ri_ += data_size - 1;
            return;
            }
        }
    }



void Disasm::Putting_labels()
    {
    for (int s = 0; s < labels_top_; s++)
        if (labels_[s].pointer_ == ri_)
            {
            Writing_word(labels_[s].name_);
            Putchar('\n');
            }
    }



void Disasm::Filling_disasm_file()
    {
    FILE* Disasm_prog = fopen("Disam_prog.txt", "w");
    fwrite(Output_, sizeof(char), OutputSize_, Disasm_prog);
    fclose(Disasm_prog);
    }



void Disasm::Disasembling()
    {
    for (ri_ = 0; ri_ <  InputSize_; ri_++, Output_[wi_] = '\n', wi_++)
        {
        Putting_labels();
        printf("Input_[ri_ = %d] = %d\n", ri_, Input_[ri_]);
        switch (Input_[ri_])
            {
            case CMD_PUSH:
                Writing_word("push");
                Arg_func_handling();
                break;

            case CMD_ADD:
                Writing_word("add");
                break;

            case CMD_MUL:
                Writing_word("mul");
                break;

            case CMD_SUB:
                Writing_word("sub");
                break;

            case CMD_DIV:
                Writing_word("div");
                break;

            case CMD_MOD:
                Writing_word("mod");
                break;

            case CMD_PRINTF:
                Writing_word("printf");
                break;

            case CMD_SCANF:
                Writing_word("scanf");
                break;

            case CMD_EXIT:
                Writing_word("exit");
                break;

            case CMD_DEBUG:
                Writing_word("debug");
                break;

            case CMD_divD:
                Writing_word("divD");
                break;

            case CMD_SQRT:
                Writing_word("sqrt");
                break;
                
            case CMD_PUSH_R:
                Writing_word("push");
                Reg_func_handling();
                break;

            case CMD_POP_R:
                Writing_word("pop");
                Reg_func_handling();
                break;

            case CMD_PUSH_RAM_REG:
                Writing_word("push");
                RAM_REG_func_handling();
                break;

            case CMD_POP_RAM_REG:
                Writing_word("pop");
                RAM_REG_func_handling();
                break;

            case CMD_PUSH_RAM_NUM:
                Writing_word("push");
                RAM_NUM_func_handling();
                break;

            case CMD_POP_RAM_NUM:
                Writing_word("pop");
                RAM_NUM_func_handling();
                break;

            case CMD_JMP:
                Writing_word("jmp");
                jump_handling();
                break;

            case CMD_JMPB:
                Writing_word("jmpb");
                jump_handling();
                break;

            case CMD_JMPA:
                Writing_word("jmpa");
                jump_handling();
                break;

            case CMD_JMPE:
                Writing_word("jmpe");
                jump_handling();
                break;
                
            case CMD_JBE:
                Writing_word("jbe");
                jump_handling();
                break;
                
            case CMD_JAE:
                Writing_word("jae");
                jump_handling();
                break;
                
            case CMD_JNE:
                Writing_word("jae");
                jump_handling();
                break;
                
            default:
                printf("command has not found\nri_ = %d, Input_[ri_] = %d\n", ri_, Input_[ri_]);
                break;
            }
        }
    OutputSize_ = wi_;
    wi_ = 0;
    ri_ = 0;
    Putchar('\0');
    }



int main(int argc, char* argv[])
    {
    Disasm pr(argv[1]);
    pr.Disasembling();
    printf("\nSecond Disassembling\n");
    pr.Disasembling();
    pr.Filling_disasm_file();
    }