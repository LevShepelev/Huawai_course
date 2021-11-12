#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/// \brief Safety calloc
void* mycalloc (int size_of_elem, int size)
    {
    char* mem = (char*) calloc (size, size_of_elem);
    if (mem == NULL) 
        {
        printf("Calloc error\n");
        exit (EXIT_FAILURE);
        }
    return mem;
    }

#define Check(left_op, cond, right_op, message, return_value) do { \
    if (left_op cond right_op) \
    {                          \
    printf("%s", message);     \
    return return_value;       \
    }                          \
    } while(0)
    


/// \brief The function check all stack paramaters and return errors   
// TODO: не информативное название для переменной
int verification(Stack* s1)
    {
    unsigned long long hash_check = 0;

    Check(s1, ==, NULL, "NULL pointer\n", NULL_POINTER);
    Check(s1 -> top, >=, s1 -> capacity, "Stack was destructed\n", DESTRUCTED_STACK);
    Check(s1 -> arr, ==, NULL, "Stack was destructed\n", DESTRUCTED_STACK);

    #ifdef Canaries_guard
        Check(s1 -> canary_left, !=, canary, "Stack was attacked from left\n", SMASHED_STACK_LEFT);
        Check(s1 -> canary_right, !=, canary, "Stack was attacked from right\n", SMASHED_STACK_RIGHT);
        Check(*((Canary_t*) s1 -> arr), !=, canary, "Stack array was attacked from left\n", SMASHED_STACK_ARRAY_LEFT);
        Check(*(Canary_t*)(Canary_shift(s1 -> arr) + s1 -> capacity), !=, canary, "Stack array was attacked from right\n", SMASHED_STACK_ARRAY_RIGHT);
    #endif
    #ifdef Hash_guard
        for (int i = 0; i < sizeof(Stack) - 2 * sizeof(Canary_t); i++)
            hash_check += *((unsigned char*)(&(s1 -> top)) + i);
        for (int i = 0; i < s1 -> capacity; i++)
            hash_check += *((unsigned char*)(s1 -> arr) + i);

        Check(s1 -> hash_guard, !=, hash_check, "Stack was attacked, hash was changed\n", WRONG_HASH);
    #endif
    return 0;
    }



/// \brief The function constuct stack with (stsize) elemts filled with poison
Stack* stack_construct(int stsize)
    {
    Stack* s1 = (Stack*) mycalloc(sizeof(Stack), 1);
    assert(s1 != NULL);
    #ifndef Canaries_guard
        s1 -> arr = (Data_t*) mycalloc (stsize, sizeof(Data_t));
    #else
        s1 -> arr = (Data_t*) mycalloc (stsize * sizeof(Data_t) + 2 * sizeof(Canary_t), sizeof(char));
        *((Canary_t*) s1 -> arr) = canary;
        *(Canary_t*)(Canary_shift(s1 -> arr) + stsize) = canary;
    #endif
    
    s1 -> canary_left = canary;
    s1 -> canary_right = canary;
    s1 -> top = -1;
    s1 -> capacity = stsize;
    s1 -> hash_guard = 0;
    for (int i = 0; i < stsize; i++)
        {
        #ifndef Canaries_guard
            *(s1 -> arr + i) = poison;
        #else 
            *(Canary_shift(s1 -> arr) + i) = poison;
        #endif
        }

    #ifdef Hash_guard
        for (int i = 0; i < sizeof(Stack) - 2 * sizeof(Canary_t); i++)
            s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
        for (int i = 0; i < s1 -> capacity; i++)
            s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);
    #endif
    return s1;
    }



/// \brief The function free memory and clear stack parameters
void stack_destruct(Stack* s1)
    {
    assert(s1 != NULL);
    if (s1 -> arr == NULL)
        {
        printf("Stack already was destucted\n");
        return;
        }
    free(s1 -> arr);
    s1 -> arr = NULL;
    s1 -> capacity = 0;
    free(s1);
    }



/// \brief The function add element inp to stack
void stack_push(Stack* s1, Data_t inp)
    {
    assert(s1 != NULL);
    if (verification (s1) == 0)
        {
        if (s1 -> top >= s1 -> capacity - 1)
            {
            Data_t* temp = s1 -> arr;
            #ifndef Canaries_guard
                temp = (Data_t*) realloc (temp, ((s1 -> capacity + 10) * 2) * sizeof(Data_t));
            #else 
                temp = (Data_t*) realloc (temp, ((s1 -> capacity + 10) * 2) * sizeof(Data_t) + 2 * sizeof(Canary_t));
            #endif
            if (temp != NULL)
                {
                s1 -> arr = temp;
                s1 -> capacity = (s1 -> capacity + 10) * 2;
                #ifdef Canaries_guard
                    *(Canary_t*)(Canary_shift(s1 -> arr) + s1 -> capacity) = canary;
                #endif
                }
            else
                {
                printf("failed memory reallocation\n");
                return;
                }
            }
        // TODO: стрелочка имеет самый большой приоритет операции, нужно убрать скобки
        (s1 -> top)++;
        #ifndef Canaries_guard
            *(s1 -> arr + s1 -> top) = inp;
        #else 
            *(Canary_shift(s1 -> arr) + s1 -> top) = inp;
        #endif

        #ifdef Hash_guard
            s1 -> hash_guard = 0;
            for (int i = 0; i < sizeof(Stack) - 2 * sizeof(Canary_t); i++)
                s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
            for (int i = 0; i < s1 -> capacity; i++)
                s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);
        #endif
        }
    }



/// \brief The function delete top element from stack and return it's value
Data_t stack_pop(Stack* s1)
    {
    assert(s1 != NULL);
    if (verification (s1) != 0)
        return poison;

    if (s1 -> top <= -1)
        {
        printf("stack is empty\n");
        return poison;
        }
    
    #ifndef Canaries_guard
        Data_t temp = *(s1 -> arr + s1 -> top);
        *(s1 -> arr + s1 -> top) = poison;
        (s1 -> top)--;
        if (s1 -> top < (s1 -> capacity) / 2)
            {
            Data_t* temp = s1 -> arr;
            temp = (Data_t*) realloc(temp, (s1 -> capacity / 1.5 + 1) * sizeof(Data_t));
            if (temp != NULL)
                {
                s1 -> arr = temp;
                s1 -> capacity = s1 -> capacity / 1.5 + 1;
                }
            }
    #else
        Data_t temp = *(Canary_shift(s1 -> arr) + s1 -> top);
        *(Canary_shift(s1 -> arr) + s1 -> top) = poison;
        (s1 -> top)--;
        if (s1 -> top < (s1 -> capacity) / 2)
            {
            Data_t* temp = s1 -> arr;
            temp = (Data_t*) realloc (temp, ((s1 -> capacity / 1.5 + 1) * sizeof(Data_t) + 2 * sizeof(Canary_t)));
            if (temp != NULL)
                {
                s1 -> arr = temp;
                s1 -> capacity = (s1 -> capacity / 1.5 + 1);
                *(Canary_t*)(Canary_shift(s1 -> arr) + s1 -> capacity) = canary;
                }
            }
    #endif

    #ifdef Hash_guard
        s1 -> hash_guard = 0;
        for (int i = 0; i < sizeof(Stack) - 2 * sizeof(Canary_t); i++)
            s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
        for (int i = 0; i < s1 -> capacity; i++)
            s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);
    #endif

    return temp;
    }



/// \brief The function prints information about stack
void print_stack(Stack* s1)
    {
    assert(s1 != NULL);
    if (verification(s1) != 0)
        return;
        
    printf("Stack adress = %p\n"
            "Stack capacity = %d\n"
            "top = %d\n"
            "array adress = %p\n"
            "hash = %llu\n"
            "stack contains:\n", s1, s1 -> capacity, s1 -> top, s1 -> arr, s1 -> hash_guard);
    if (s1 -> top == -1)
        printf("Nothing, Stack is empty\n");
    else
        for (int i = 0; i <= s1 -> top; i++)
            {
            #ifndef Canaries_guard
                printf("array[%d] = %f\n", i, *(s1 -> arr + i));
            #else 
                printf("array[%d] = %f\n", i, *(Canary_shift(s1 -> arr) + i));
            #endif
            }
        
    }



/// \brief The function brings the stack back to its initial state
void clear_stack(Stack* s1)
    {
    assert(s1 != NULL);
    for (int i = s1 -> top; i < s1 -> capacity; i++)
        {
        #ifndef Canaries_guard
            *(s1 -> arr + i) = poison;
        #else 
            *(Canary_shift(s1 -> arr) + i) = poison;
        #endif
        }
    s1 -> top = -1;
    
    #ifdef Hash_guard
    s1 -> hash_guard = 0;
    for (int i = 0; i < sizeof(Stack) - 2 * sizeof(Canary_t); i++)
        s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
    for (int i = 0; i < s1 -> capacity; i++)
        s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);
    #endif
    }



/// \brief The function return top value without deleting that element    
Data_t stack_get(Stack* s1)
    {
    assert(s1 != NULL);
    if (verification (s1) == 0)
        {
        #ifndef Canaries_guard
        return *(s1 -> arr + s1 -> top);
        #else
        return *(Canary_shift(s1 -> arr) + s1 -> top);
        #endif
        }
    else return poison;
    }


/// \brief THe function return pointer of array with Data_t, we skip Array_canary
Data_t* Canary_shift(Data_t* arr)
    {
    return (Data_t*)((char*) arr + sizeof(Canary_t));
    }