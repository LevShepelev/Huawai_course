#include "Onegin.h"

int main()
    {
    struct str* text = NULL;
    int numb = 0;
    char* buf = starting(&numb, &text);

    filltext(buf, text, numb);
    writing_to_file(buf, text, numb);
    finishing(buf, text);
    }