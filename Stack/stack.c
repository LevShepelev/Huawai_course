#include"stack.h"
void* mycalloc (int size_of_elem, int size)
    {
    char* mem = (char*) calloc (size, size_of_elem);
    if (mem == NULL) 
        {
        printf("Calloc error\n");
        exit (EXIT_FAILURE); // используй EXIT_FAILURE вместо 1
        }
    return mem;
    }
    
int verification(mystack* s1)
    {
    unsigned long long hash_check = 0;
    if (s1 == NULL)
        {
        printf("NULL pointer\n");
        return 5; // для кодов ошибок сделать enum
        }
    if (s1 -> top >= s1 -> capacity)
        {
        printf("stack overflow\n");
        return 1;
        }
    if (s1 -> arr == NULL)
        {
        printf("Stack was destructed\n");
        return 2;
        }
    if (s1 -> canary_left != canary)
        {
        printf("Stack was attacked from left\n");
        return 3;
        }
    if (s1 -> canary_right != canary)
        {
        printf("Stack was attacked from right\n");
        return 4;
        }
    for (int i = 0; i < sizeof(mystack) - 2 * sizeof(unsigned long long); i++)
        hash_check += *((unsigned char*)(&(s1 -> top)) + i);
    for (int i = 0; i < s1 -> capacity; i++)
        hash_check += *((unsigned char*)(s1 -> arr) + i);

    if (s1 -> hash_guard != hash_check)
        {
        printf("Stack was attacked, hash was changed\n");
        print_stack(s1);
        exit (5);
        }

    return 0;
    }

// поправить кодстайл --- переменные называем с маленькой буквы Stsize
void stack_construct(mystack* s1, int Stsize)
    {
    assert(s1 != NULL);
    s1 -> canary_left = canary;
    s1 -> canary_right = canary;
    s1 -> arr = (data*) mycalloc (Stsize, sizeof(data));
    s1 -> top = -1;
    s1 -> capacity = Stsize;
    s1 -> hash_guard = 0;
    for (int i = 0; i < Stsize; i++)
        *(s1 -> arr + i) = poison;

    // сделай typedef для канареечных переменных
    for (int i = 0; i < sizeof(mystack) - 2 * sizeof(unsigned long long); i++)
        s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
    for (int i = 0; i < s1 -> capacity; i++)
        s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);

    }

void stack_destruct(mystack* s1)
    {
    assert(s1 != NULL);
    free(s1 -> arr);
    s1 -> arr = NULL;
    s1 -> capacity = 0;
    }

void stack_push(mystack* s1, data inp)
    {
    assert(s1 != NULL);
    //printf("pushed = %lf\n", inp);
    if (verification (s1) == 0)
        {
        if (s1 -> top >= s1 -> capacity - 1)
            {
            data* temp = s1 -> arr;
            temp = (data*) realloc (temp, ((s1 -> capacity + 10) * 2)*sizeof(data));
            if (temp != NULL)
                {
                s1 -> arr = temp;
                s1 -> capacity = (s1 -> capacity + 10) * 2;
                }
            else
                {
                printf("failed memory reallocation\n");
                stack_push(s1, inp);
                return;
                }
            }
        (s1 -> top)++;
        *((s1 -> arr) + (s1 -> top)) = inp;

        s1 -> hash_guard = 0;
        for (int i = 0; i < sizeof(mystack) - 2 * sizeof(unsigned long long); i++)
            s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
        for (int i = 0; i < s1 -> capacity; i++)
            s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);
        }
    }

data stack_pop(mystack* s1)
    {
    assert(s1 != NULL);
    if (verification (s1) == 0)
        {
        if (s1 -> top > -1)
            {
            data temp = *((s1 -> arr) + (s1 -> top));
            *(s1 -> arr + s1 -> top) = poison;
            (s1 -> top) --;
            if (s1 -> top < (s1 -> capacity) / 2)
                {
                data* temp = s1 -> arr;
                temp = (data*) realloc(temp, (s1 -> capacity / 1.5 + 1) * sizeof(data));
                if (temp != NULL)
                    {
                    s1 -> arr = temp;
                    s1 -> capacity = (s1 -> capacity / 1.5 + 1);
                    }
                }

            s1 -> hash_guard = 0;
            for (int i = 0; i < sizeof(mystack) - 2 * sizeof(unsigned long long); i++)
                s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
            for (int i = 0; i < s1 -> capacity; i++)
                s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);

            return temp;
            }
        else printf("stack is empty\n");
        }
    else return poison;
    }

void print_stack(mystack* s1)
    {
    assert(s1 != NULL);
    if (verification(s1) == 0)
        {
        printf("Stack adress = %p\nStack capacity = %d\ntop = %d\narray adress = %p\nhash = %llu\nstack contains:\n", s1, s1 -> capacity, s1 -> top, s1 -> arr, s1 -> hash_guard);
        if (s1 -> top == -1)
            printf("Nothing, Stack is empty\n");
        else
        for (int i = 0; i <= s1 -> top; i++)
            printf("array[%d] = %lf\n", i, *(s1 -> arr + i));
        }
    }
void clear_stack(mystack* s1)
    {
    assert(s1 != NULL);
    for (int i = s1 -> top; i < s1 -> capacity; i++)
        *(s1 -> arr + i) = poison;
    s1 -> top = -1;
    s1 -> hash_guard = 0;
    for (int i = 0; i < sizeof(mystack) - 2 * sizeof(unsigned long long); i++)
        s1 -> hash_guard += *((unsigned char*)(&(s1 -> top)) + i);
    for (int i = 0; i < s1 -> capacity; i++)
        s1 -> hash_guard += *((unsigned char*)(s1 -> arr) + i);
    }
data stack_get(mystack* s1)
    {
    assert(s1 != NULL);
    if (verification (s1) == 0)
        return *((s1 -> arr) + (s1 -> top));
    else return poison;
    }

int main()
    {
    mystack s1;
    stack_construct(&s1, 10);
    stack_push(&s1, 12);
    }