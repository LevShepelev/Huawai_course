#include "Stack.h"

// TODO: сделать простой тестик
// TODO: написать документацию с тегами \brief, \detail, \param, \return, (optional) \note, (optional) \warning
// \param [in] <name> <description>
// example:
// \param  [in]  elem_size  Size of element that will store in stack.

int main()
    {
    Stack* s1 = stack_construct(1);
    for (int i = 0; i < 100; i++)
        stack_push(s1, 10);
    for (int i = 0; i < 60; i++)
        stack_pop(s1);
    }