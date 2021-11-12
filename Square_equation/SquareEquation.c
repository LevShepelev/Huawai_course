#include "Square_equation.h"
#include <stdio.h>


int main()
    {
    Unit_tests();
    printf ("Square Equation Solver 2020\nWrite coefficient a, b, c\n");
    double mass[3];
    double x1 = 0, x2 = 0;
    for (int i = 0; i < 3; i++)
        scanf("%lg", &(mass[i]));
    int nRoots = SolveSquare (mass[0], mass[1], mass[2], &x1, &x2);
    if (nRoots == 2)
        printf ("x1 = %+lg\n"  "x2 = %+lg\n", x1, x2);
        
    if  (nRoots == 1)
        printf ("x = %lg\n", x1) ;
        
    return 0;
    }





