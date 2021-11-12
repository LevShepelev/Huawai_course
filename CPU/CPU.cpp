#include "CPU.h"

void CPU::Push_handling()
    {
    assert(this != NULL);
    ID++;
    Decryption_number_to_rdi();
    stack_push(stack, reg[rdi]);
    }



CPU::CPU(char* Commands_file_name)
    {
    assert(this != NULL);
    for (int s = 0; s < number_of_registers; s++)
        reg[s] = 0;
    stack = stack_construct(10);
    stack_pointer = stack_construct(10);
    RAM  = (void*) mycalloc(RAM_size,  sizeof(char));
    GRAM = (char*) mycalloc(GRAM_size, sizeof(char));
    struct stat statistica;
    FILE* Commands = fopen(Commands_file_name, "r");
    int stat_error = stat (Commands_file_name, &statistica);
    assert(stat_error == 0);
    buf_size = statistica.st_size;
    buf = (char*) mycalloc ((buf_size + 1), sizeof(char));
    fread ((void*) buf, sizeof(char), buf_size, Commands);
    fclose(Commands);
    }


CPU::~CPU()
    {
    free(RAM);
    free(GRAM);
    free(buf);
    stack_destruct(stack);
    stack_destruct(stack_pointer);
    }



void CPU::Decryption_number_to_rdi()
    {
    reg[rdi] = *(double*)(buf + ID);
    ID += data_size - 1;
    }



void CPU::Jump_handling()
    {
    assert(this != NULL);
    Decryption_number_to_rdi();
    ID = (int) reg[rdi] - 1;
    }



void CPU::Execution()
    {
    assert(this != NULL);
    for (ID = 0; ID < buf_size; ID++)
        {   
        //printf("buf = %d ID = %d\n", buf[ID], ID);
        switch (buf[ID])
            {
            case CMD_EXIT:
                printf("finishing program\n");
                return;

            case CMD_PUSH:         
                Push_handling();
                break;

            case CMD_PUSH_R:
                ID++;
                stack_push(stack, reg[buf[ID]]);
                break;
            
            case CMD_PUSH_RAM_REG:
                ID++;
                assert(reg[buf[ID]] < RAM_size);
                stack_push(stack, ((double*) RAM)[(int) reg[buf[ID]]]);
                break;
            
            case CMD_PUSH_RAM_NUM:
                ID++;
                Decryption_number_to_rdi();
                assert(reg[rdi] < RAM_size);
                stack_push(stack, ((double*) RAM)[(int) reg[rdi]]);
                break;
            
            case CMD_POP_GRAM_REG:
                ID++;
                assert(reg[buf[ID]] < GRAM_size);
                GRAM[(int) reg[buf[ID]]] = stack_pop(stack);
                break;

            case CMD_POP_GRAM_NUM:
                ID++;
                Decryption_number_to_rdi();
                assert(reg[rdi] < GRAM_size);
                GRAM[(int) reg[rdi]] = stack_pop(stack);
                break;
            
            case CMD_GRAPHICS:
                for (int y = 0; y < GRAM_size / Screen_width; y++)
                    {
                    for (int x = 0; x < Screen_width; x++)
                        printf("%c", GRAM[Screen_width * y + x]);
                    printf("\n");
                    }
                break;

            case CMD_POP_R:
                ID++;
                reg[buf[ID]] = stack_pop(stack);
                break;
            
            case CMD_POP_RAM_REG:
                ID++;
                assert(reg[buf[ID]] < RAM_size);
                ((double*) RAM)[(int) reg[buf[ID]]] = stack_pop(stack);
                break;

            case CMD_POP_RAM_NUM:
                ID++;
                Decryption_number_to_rdi();
                assert(reg[rdi] < RAM_size);
                ((double*) RAM)[(int) reg[rdi]] = stack_pop(stack);
                break;

            case CMD_ADD:
                stack_push(stack, stack_pop(stack) + stack_pop(stack));
                break;

            case CMD_MUL:
                stack_push(stack, stack_pop(stack) * stack_pop(stack));
                break;

            case CMD_SUB:
                stack_push(stack, stack_pop(stack) - stack_pop(stack));
                break;

            case CMD_DIV:
                stack_push(stack, (long long int) (stack_pop(stack) / stack_pop(stack)));
                break;

            case CMD_MOD:
                stack_push(stack, (long long int) stack_pop(stack) % (long long int)stack_pop(stack));
                break;
            
            case CMD_divD:
                stack_push(stack, stack_pop(stack) / stack_pop(stack));
                break;

            case CMD_SQRT:
                stack_push(stack, sqrt(stack_pop(stack)));
                break;

            case CMD_PRINTF:
                printf("%f\n", stack_get(stack));
                break;

            case CMD_SCANF:
                scanf("%lg", &(reg[rdi]));
                stack_push(stack, reg[rdi]);
                break;

            case CMD_DEBUG:
                print_stack(stack);
                print_stack(stack_pointer);
                for (int i = 0; i < 10; i++)
                    printf("Ram = %lf\n", ((double*)RAM)[i]);
                for (int i = 0; i < 6; i++)
                    printf("reg[%d] = %lf\n", i, reg[i]);
                break;
            
            case CMD_CALL:
                ID++;
                stack_push(stack_pointer, (ID) + data_size - 1);
                Jump_handling();
                break;

            case CMD_RET:
                ID = (int) stack_pop(stack_pointer);
                break;

            case CMD_JMP:
                ID++;
                Jump_handling();
                break;

            case CMD_JMPB:
                ID++; 
                if (stack_pop(stack) < reg[rsi])
                    Jump_handling();
                else
                    ID += data_size - 1;
                break;

            case CMD_JMPA:
                ID++; 
                if (stack_pop(stack) > reg[rsi])
                    Jump_handling();
                else
                    ID += data_size - 1;
                break;

            case CMD_JMPE:
                ID++; 
                if (stack_pop(stack) == reg[rsi])
                    Jump_handling();
                else
                    ID += data_size - 1;
                break;

            case CMD_JBE:
                ID++; 
                if (stack_pop(stack) <= reg[rsi])
                    Jump_handling();
                else
                    ID += data_size - 1;
                break;

            case CMD_JAE:
                ID++; 
                if (stack_pop(stack) >= reg[rsi])
                    Jump_handling();
                else
                    ID += data_size - 1;
                break;

            case CMD_JNE:
                ID++; 
                if (stack_pop(stack) != reg[rsi])
                    Jump_handling();
                else
                    ID += data_size - 1;
                break;

            default:
                //scanf("%c");
                printf("CPU ERROR\n");
            }
        }
    }



int main(int argc, char* argv[])
    {
    CPU c1(argv[1]);
    c1.Execution();
    }