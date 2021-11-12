#ifndef _STACKCPP_H
#define _STACKCPP_H

#include <cstdio>
#include <stdlib.h>
#include <assert.h>
#include <cstddef>
#include <utility>
#include <iostream>
#include <string>

#define poison NULL
#define canary 0xDEADBEEFBEEFDEAD

template <class T>
class Stack
        {
        private:
                unsigned long long canary_left_;
                unsigned long long hash_guard_;
                int top_;
                int capacity_; 
                T* arr_; 
                unsigned long long canary_right_;
        public:
                const T& operator[] (int n) const;    
                void push(T inp);
                int  verification();
                int Top();
                Stack<T> (int Stsize);
                Stack<T> (Stack<T>&& obj);
                Stack<T> (Stack<T> const& obj);
                Stack<T> operator = (const Stack<T>& stk);
                Stack<T> operator = (Stack<T> && obj);     
                ~Stack<T>();
                T pop();
                void Dump();
                void clear();
                T get();
        };

//void* mycalloc (int size, int size_of_elem);
template <class T>
void swap (Stack<T>& s1, Stack<T>& s2);

#endif