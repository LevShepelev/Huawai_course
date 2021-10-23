#ifndef ONEGIN
#define ONEGIN

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <ctype.h>

struct str{char* s; int l;};


int linecount(char* buf, const int value);
void filltext(char* buf, struct str* text, const int numb);
int cmp1 (const void* qtext1, const void* qtext2);
int cmp2 (const void* qtext1, const void* qtext2);
void textprintf(struct str* text, int numb, FILE* fout);
void length_of_strings(const char* buf, struct str* text, const int numb, int value);
char* starting(int* numb, struct str** text);
void* my_calloc (int size_of_elem, int size);
void writing_to_file(char* buf, struct str* text, int numb);
void finishing(char* buf, struct str* text);
void Qsort(void* arr, int arr_size, int elem_size, int (*comparator) (const void *, const void *));

#endif