#ifndef STACK
#define STACK

#define Hash_guard
#define Canaries_guard

#define poison -666
#define canary 0xDEADBEEFBEEFDEAD

typedef double Data_t;
typedef unsigned long long Canary_t;

typedef struct str_stack {
    #ifdef Canaries_guard
        Canary_t canary_left;
    #endif
    unsigned long long hash_guard;
    int top; 
    int capacity;
    Data_t* arr;
    #ifdef Canaries_guard
        Canary_t canary_right;
    #endif
    } Stack;


void stack_push(Stack* s1, Data_t inp);
int verification(Stack* s1);
Stack* stack_construct(int Stsize);
void stack_destruct(Stack* s1);
Data_t stack_pop(Stack* s1);
void print_stack(Stack* s1);
void clear_stack(Stack* s1);
Data_t stack_get(Stack* s1);
void* mycalloc (int size_of_elem, int size);
Data_t* Canary_shift(Data_t* arr);

enum Errors
    {
    STACK_OVERFLOW            = 1,
    DESTRUCTED_STACK          = 2,
    SMASHED_STACK_LEFT        = 3,
    SMASHED_STACK_RIGHT       = 4,
    NULL_POINTER              = 5,
    WRONG_HASH                = 6,
    SMASHED_STACK_ARRAY_LEFT  = 7,
    SMASHED_STACK_ARRAY_RIGHT = 8
    };

#endif