#include "Onegin.h"

/// \brief The function open file and fill bufer
char* starting(int* numb, struct str** text)
    {
    printf("Welcome to text sorting machine...\n");
    FILE* fin = fopen("Onegin_input.txt", "r");
    struct stat statistica;
    int stat_error = stat ("Onegin_input.txt", &statistica);
    assert(stat_error == 0);

    char* buf = (char*) my_calloc (statistica.st_size + 1, sizeof(char));

    fread ((void*) buf, sizeof(char), statistica.st_size, fin);  // recording text in bufer

    *numb = linecount(buf, statistica.st_size); //finding the number of strings in the text

    (*text) = (struct str*) my_calloc (*numb, sizeof(struct str));
    length_of_strings(buf, *text, *numb, statistica.st_size);

    fclose (fin);

    return buf;
    }
    

/// \brief general comparator for strings
int cmp1 (const void* qtext1, const void* qtext2)
    {
    struct str text1 = *((struct str*)qtext1);
    struct str text2 = *((struct str*)qtext2);
    int i = 0;
    int j = 0;
    while ((i < text1.l) && (j < text2.l))
        {
        // isalpha()
        while (!isalpha(text1.s[i]) && (i < text1.l) && (j < text2.l))
            i++;
        while (!isalpha(text2.s[j]) && (i < text1.l) && (j < text2.l))
            j++;
        if (text1.s[i] < text2.s[j]) return -1;
        if (text1.s[i] > text2.s[j]) return 1;

        i++;
        j++;
        }
    if (text1.l < text2.l) return -1;
    if (text1.l > text2.l) return 1;
    if (text1.l == text2.l) return 0;
    }



/// \brief Rhyme Comparator, it compare symbols of 2 strings from the end of strings
int cmp2 (const void* qtext1, const void* qtext2)
    {
    struct str text1 = *((struct str*)qtext1);
    struct str text2 = *((struct str*)qtext2);
    int i = 0;
    int j = 0;
    while ((i < text1.l) && (j < text2.l))
        {
        // isalpha()
        while (!isalpha(text1.s[text1.l - i - 1]) && (i < text1.l - 1) && (j < text2.l - 1))
            i++;
        while (!isalpha(text2.s[text2.l - j - 1]) && (i < text1.l - 1) && (j < text2.l - 1))
            j++;
        if (text1.s[text1.l - i - 1] < text2.s[text2.l - j - 1]) return -1;
        if (text1.s[text1.l - i - 1] > text2.s[text2.l - j - 1]) return 1;

        i++;
        j++;
        }
    if (text1.l < text2.l) return -1;
    if (text1.l > text2.l) return 1;
    if (text1.l == text2.l) return 0;
    }



/// \brief The function prints text to file
void textprintf(struct str* text, int numb, FILE* fout)
    {
    int n_empty = 0;
    for (n_empty = 0; text[n_empty].s[0] == '\0' || text[n_empty].s[0] == '\r' || text[n_empty].s[0] == '\n'; n_empty++)
        ;
    for (int i = n_empty; i < numb; i++)
        {
        for (int j = 0; j <= text[i].l; j++)
            {
            if (*(text[i].s + j) != '\r' && *(text[i].s + j) != '\0')
                fprintf(fout, "%c", *(text[i].s + j));
                
            }
        }
    }   



/// \brief The function count number of lines in the text
int linecount(char* buf, const int value)
    {
    int linecounter = 0;

    for (int i = 0; i <= value; i++)
        {
         // считаем сколько строк
        if (buf[i] == '\n')
            linecounter++;
        }
    return linecounter + 1;
    }



/// \brief The function fill array of struct from bufer
void filltext(char* buf, struct str* text, const int numb)
    {
    int j = 0;
    int i = 0;

    for (i = 0; i < numb; i++)
        {
        text[i].s = buf + j;
        j += text[i].l + 1;
        }
    }



/// \brief The function count length of each string in text
void length_of_strings(const char* buf, struct str* text, const int numb, int value)
    {
    for (int i = 0; i < numb; i++)
        for (int j = 0; j < value; j++)
            {   
            if ((buf[j] == '\n') || (buf[j] == '\0'))
                {
                text[i].l = j;
                buf += j + 1;
                break;
                }
            }
    }




/// \brief The function allocate memory safely
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


/// \brief The function writes final text to file
void writing_to_file(char* buf, struct str* text, int numb)
    {
    assert(text);
    assert(buf);

    FILE* fout = fopen("Onegin_output.txt", "w");

    // напиши свою версию qsort с прототипом таким же, как и в стандартной либе
    Qsort((void*)text, numb, sizeof(struct str), cmp1); // Sorting the text using the general comparator and printing the text

    fprintf(fout, "SORTED TEXT\n");
    textprintf(text, numb, fout);

    Qsort((void*)text, numb, sizeof(struct str), cmp2);// Sorting the text using the rhyme comparator and printing the text

    fprintf(fout, "\nRHYME SORTED TEXT\n");
    textprintf(text, numb, fout);

    fprintf(fout, "\nOriginal Text\n%s", buf);
    fclose(fout);
    }


/// \brief The function free memory
void finishing(char* buf, struct str* text)
    {
    printf("Well done !\n");
    free(buf);
    free(text);
    }

/// \brief My qsort
void Qsort(void* arr, int arr_size, int elem_size, int (*comparator) (const void *, const void *))
    {
    arr_size--;
    if (arr_size == 0)
        return;
    char buf[elem_size];
    int first = 0, last = arr_size - 1;
    int piv = last / 2;
    do 
        {
        
        while (comparator(arr + first * elem_size, arr + piv * elem_size) == -1)
            {
            first++;
            }
        while (comparator(arr + last * elem_size, arr + piv * elem_size) == 1)
            {
            last--;
            }
        if (first <= last)
            {
            memcpy(buf, arr + first * elem_size, elem_size);
            memcpy(arr + first * elem_size, arr + last * elem_size, elem_size);
            memcpy(arr + last * elem_size, buf, elem_size);
            first++;
            last--;
            }
        } while (first <= last);

    if (last > 0)
        {
        Qsort (arr, last, elem_size, comparator);
        }
    if (first < arr_size)
        {
        Qsort (arr + first * elem_size, arr_size - first, elem_size, comparator);
        }
    } 
    

