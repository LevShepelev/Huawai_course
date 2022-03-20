#include "../include/Assembler.h"



void* my_calloc (FILE* log_file, int size_of_elem, int size)
    {
    char* mem = (char*) calloc (size, size_of_elem);
    if (mem == NULL) 
        {
        printf("Calloc error\n");
        fprintf(log_file, "Calloc error\n");
        exit (1);
        }
    return mem;
    }



Assemb::Assemb(char* Input_file_name, FILE* CMD_ENG)
    {
    assert(this != NULL);
    log_file = fopen("log.txt", "w");
    struct stat statistica;
    int stat_error = stat (Input_file_name, &statistica);
    assert(stat_error == 0);

    labels = (mark_t*) my_calloc (log_file, labels_size, sizeof(mark_t));

    lexemm = (char*) my_calloc (log_file, max_lexemm_length, sizeof(char));

    Input = (char*) my_calloc (log_file, (statistica.st_size + 1), sizeof(char));

    fread ((void*) Input, sizeof(char), statistica.st_size, CMD_ENG);

    Output = (char*) my_calloc (log_file, data_size * (statistica.st_size + 1), sizeof(char));

    buf_size = statistica.st_size;

    line_counter = 1;

    for (int z = 0; z < labels_size; z++)
        {
        (labels[z]).name = (char*) my_calloc (log_file, Label_name_size, sizeof(char));
        (labels[z]).pointer = -1;
        }
    }



Assemb::~Assemb()
    {
    assert((this != NULL) || (labels != NULL));
    for (int z = 0; z < labels_size; z++)
        free((labels[z]).name); 
    free(labels);
    free(Input);
    free(Output);
    Input  = (char*) 1;
    Output = (char*) 1;
    labels = (mark_t*) 1;
    fclose(log_file);
    printf("Succesfully compiled\n");
    }



void Assemb::Write_number_like_chars(double number)
    {
    assert(this != NULL);
    *(double*)(Output + wi) = number;
    wi += data_size - 1;
    }



void Assemb::Putting_number_into_machine_code()
    {
    char *endptr, numb_byte[data_size];
    double number = strtod(lexemm, &endptr);
    //printf("strtod = %lf\n", number);
    if (endptr - (lexemm) != strlen(lexemm))
        Error_report("ERROR: incorrect input in function \"push\", line %d\n");
    Write_number_like_chars(number);
    }



void Assemb::Error_report(const char* message)
    {
    assert(this != NULL);
    printf(message, line_counter);
    fprintf(log_file, message, line_counter);
    exit (1);
    }
    


void Assemb::Get_lexemm()
    {
    assert(this != NULL);
    int k = 0;
    for (k = 0; (Input[ri] != ' ') && (Input[ri] != '\n') && (Input[ri] != ';') && (ri < buf_size); ri++, k++)
        {
        if (Input[ri] == '[')
            k--;
        else if (Input[ri] == ']')
            {
            IsCommandRam = 1;
            lexemm[k] = '\0';
            }
        else lexemm[k] = Input[ri];
        }
    lexemm[k] = '\0';

    if (Input[ri] == ';')
        for (; Input[ri] != '\n'; ri++) // comments
            ;
    do 
        {
        if (Input[ri] == '\n')
            (line_counter)++;
        ri++;
        } while (((Input[ri] == ' ') || (Input[ri] == '\n')) && ri < buf_size - 1);
    }



void Assemb::Crypting_name_of_register()
    {
    if (!strcmp(lexemm, "rax"))
        Output[wi] = CMD_RAX;

    else if (!strcmp(lexemm, "rbx"))
        Output[wi] = CMD_RBX;

    else if (!strcmp(lexemm, "rcx"))
        Output[wi] = CMD_RCX;

    else if (!strcmp(lexemm, "rdx"))
        Output[wi] = CMD_RDX;

    else if (!strcmp(lexemm, "rsi"))
        Output[wi] = CMD_RSI;

    else 
        Error_report("ERROR: incorrect name of register, line %d\n");
    }



void Assemb::RAM_handling(int first_cmd, int sec_cmd)
    {
    if ((lexemm[0] >= '0') && (lexemm[0] <= '9'))
        {
        Output[wi] = first_cmd;
        wi++;
        Putting_number_into_machine_code();
        }
    else
        {
        Output[wi] = sec_cmd;
        wi++;
        Crypting_name_of_register();
        }
    IsCommandRam  = 0;
    }



void Assemb::Push_handling()
    {
    assert(this != NULL);
    Get_lexemm();
    if (IsCommandRam)
        RAM_handling(CMD_PUSH_RAM_NUM, CMD_PUSH_RAM_REG);
    
    else if (lexemm[0] == 'r')
        {
        Output[wi] = CMD_PUSH_R;
        wi++;
        Crypting_name_of_register();
        }
    else
        {
        Output[wi] = CMD_PUSH;
        wi++;
        Putting_number_into_machine_code();
        }
    }



void Assemb::Pop_handling(int IsGraphics)
    {
    assert(this != NULL);
    Get_lexemm();
    if (IsCommandRam)
        if (IsGraphics)
            RAM_handling(CMD_POP_GRAM_NUM, CMD_POP_GRAM_REG);
        else
            RAM_handling(CMD_POP_RAM_NUM, CMD_POP_RAM_REG);
    else if (lexemm[0] == 'r')
        {
        Output[wi] = CMD_POP_R;
        wi++;
        Crypting_name_of_register();
        }
    }



void Assemb::Label_handling()
    {
    assert((this != NULL) || (labels != NULL));
    int i = 0;
    if (strchr(lexemm, ':') - lexemm == strlen(lexemm) - 1)
        {
        for (i = 0; i < labels_size; i++)
            if (strcmp(lexemm, labels[i].name) == 0)
                {
                wi--;
                return;
                }
        for (i = 0; labels[i].pointer != -1; i++)
            if (i == labels_size) 
                {
                Error_report("Too many labels in program, max number %d\n");
                break;
                }            
        strcpy(labels[i].name, lexemm);
        labels[i].pointer = wi;
        wi--;
        }
    else 
        Error_report("Incorrect name of label, line %d\n");
    }



void Assemb::Jump_handling()
    { 
    assert((this != NULL) || (labels != NULL));
    int i = 0;
    wi++;
    Get_lexemm();
    for (i = 0; i < labels_size; i++)
        if (strcmp(labels[i].name, lexemm) == 0) 
            {
            Write_number_like_chars(labels[i].pointer);
            return;
            }
    if ((i == labels_size) && (assembling_level == 1)) 
        Error_report("Label has not wound line %d\n");
    else wi += data_size - 1;
    }



void Assemb::Assembling()
    {
    assert((this != NULL) || (labels != NULL));
    line_counter = 0;
    wi = 0;
    int IsExit = 0;
    for (ri = 0; ri < buf_size; wi++)
        {
        Get_lexemm();
        //printf("wi = %d lexemm = %s\n", wi,  lexemm);

        if (strchr(lexemm, ':') != NULL)
            Label_handling();
        
        else if (!strcmp(lexemm, "exit"))
            {
            Output[wi] = CMD_EXIT;
            IsExit = 1;
            }

        else if (!strcmp(lexemm, "add"))
            Output[wi] = CMD_ADD;

        else if (!strcmp(lexemm, "mul"))
            Output[wi] = CMD_MUL;

        else if (!strcmp(lexemm, "mod"))
            Output[wi] = CMD_MOD;
        
        else if (!strcmp(lexemm, "sub"))
            Output[wi] = CMD_SUB;
        
        else if (!strcmp(lexemm, "scanf"))
            Output[wi] = CMD_SCANF;
        
        else if (!strcmp(lexemm, "div"))
            Output[wi] = CMD_DIV;

        else if (!strcmp(lexemm, "divD"))
            Output[wi] = CMD_divD;

        else if (!strcmp(lexemm, "sqrt"))
            Output[wi] = CMD_SQRT;
        
        else if (!strcmp(lexemm, "printf"))
            Output[wi] = CMD_PRINTF;
        
        else if (!strcmp(lexemm, "graphics"))
            Output[wi] = CMD_GRAPHICS;
            
        else if (!strcmp(lexemm, "push"))
            Push_handling();
        
        else if (!strcmp(lexemm, "pop"))
            Pop_handling(0);

        else if (!strcmp(lexemm, "popg"))
            Pop_handling(1);

        else if (!strcmp(lexemm, "ret"))
            Output[wi] = CMD_RET;

        else if (!strcmp(lexemm, "call"))
            {
            Output[wi] = CMD_CALL;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "jmp"))
            {
            Output[wi] = CMD_JMP;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "jmpb"))
            {
            Output[wi] = CMD_JMPB;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "jmpa"))
            {
            Output[wi] = CMD_JMPA;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "jmpe"))
            {
            Output[wi] = CMD_JMPE;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "jbe"))
            {
            Output[wi] = CMD_JBE;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "jae"))
            {
            Output[wi] = CMD_JAE;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "jne"))
            {
            Output[wi] = CMD_JNE;
            Jump_handling();
            }

        else if (!strcmp(lexemm, "debug"))
            Output[wi] = CMD_DEBUG;

        else Error_report("Command has not found, line %d\n");
        }
    if (IsExit == 0) 
        Error_report("exit was not found");
    assembling_level++;
    }



int main(int argc, char* argv[])
    {
    FILE* CMD_MACHINE   = fopen("Commands.txt", "w");
    FILE* CMD_ENG       = fopen(argv[1], "r");
    Assemb pr(argv[1], CMD_ENG);
    pr.Assembling();
    printf("\nSecond Assembling\n");
    pr.Assembling();

    for (int i = 0; i < pr.wi; i++)
        fprintf(CMD_MACHINE, "%c", pr.Output[i]);

    fclose(CMD_MACHINE);
    fclose(CMD_ENG);
    }