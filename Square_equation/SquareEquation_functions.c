#include "Square_equation.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <float.h>



/// \brief Function compare double number with zero
int Is_zero (double a)
    {
    return fabs(a) < DBL_EPSILON;
    }

//-----------------------------------------------------------------------------

/// \brief Function solves linear equation
int Solvelinear (double b, double c, double* adr1)
    {
    assert(adr1 != NULL);
    printf("b = %lg c = %lg Your equation if linear, ", b, c);

    if (!Is_zero (b) && Is_zero (c))
        {
        printf ("1 root, x = %d\n", 0);
        *adr1 = 0;
        }

    if (!Is_zero (b) && !Is_zero (c))
        {
        printf ("1 root, x = %lg\n", -c / b);
        *adr1 = -c / b;
        return 1;
        }

    if (Is_zero (b) && Is_zero (c))
        {
        printf ("Infinite number of roots\n");
        return -1;
        }

    if (Is_zero (b) && !Is_zero(c))
        {
        printf ("No roots\n");
        return 0;
        }
    }

//-----------------------------------------------------------------------------

/// \brief Function Soleves Square Equation
int SolveSquare (double a, double b, double c, double* adr1, double* adr2)
    {
    assert(adr1 != NULL && adr2 != NULL);
    if (fabs (a) > DBL_EPSILON)
        {
        double discriminant = b*b - 4*a*c;
        if (!Is_zero (discriminant) && discriminant > 0)
            {
            *adr1 = (-b - sqrt (discriminant)) / (2*a);
            *adr2 = (-b + sqrt (discriminant)) / (2*a);
            return 2;
            }
        else if (Is_zero (discriminant))
            {
            *adr1 = -b / (2*a);
            return 1;
            }
        else if (!Is_zero (discriminant) && discriminant < 0)
            {
            printf ("No roots");
            return 0;
            }
        }
    else
        Solvelinear (b, c, adr1);
    return 0;
    }

//-----------------------------------------------------------------------------

/// \brief The function reads double number symbol by symbol, comma input supported, function reads (max_numb_size) digits 
void read (double* a) 
    {
    int i = 0, err = 0, aftpoint = 0, n_aft = 0, nscan = 0, npoint = 0, neg = 0;
    const int max_numb_size = 100;

    char num[max_numb_size];
    do               // Checking correctness of input
        {
        err = 0;
        scanf ("%s", num);
        if (num[0] == '-')
            neg = 1;
        if (neg == 1)
            i = 1;
        else
            i = 0;
        for ( ; num[i] != '\0'; i++)
            {
            if ((num[i] == ',') || (num[i] == '.'))
                npoint++;
            // isdigit() 
            if ((((num[i] > '9') || (num[i] < '0')) && (num[i] != '.') && (num[i] != ','))
            || (npoint > 1) || (num[0] == ',') || (num[0] == '.'))
                {
                err = 1;
                printf ("Incorrect input, try again:\n");
                npoint = 0;
                neg = 0;
                fflush(stdin);
                break;
                }
            }
        } while (err == 1);
   
    for (i = 0; ((num[i] != ',') && (num[i] != '.') && (num[i] != '\0')); i++)  // transformation of nubmer from char to double
        {
        if (num[i] == '-')
            i++;
        nscan = nscan * 10 + num[i] - '0';
        }
    if ((num[i] = ',') || (num[i] = '.'))
        {
        for (int j = i + 1; (num[j] != '\0'); j++)
            {
            n_aft++;
            aftpoint = 10 * aftpoint + num[j] - '0';
            }
        }
    if (neg == 1)
        *a = (-nscan - aftpoint / pow (10, n_aft));
    else
        *a = (nscan + aftpoint / pow (10, n_aft));
    }

/// \brief Unit_testing_system
void Unit_tests()
    {
    double test[3][6] = {{1, -6, 9, 3, 0, 1},
                         {0, 0, 0, 0, 0, -1},
                         {1, -3, 2, 1, 2, 2}};
    double x1 = 0, x2 = 0;

    for (int i = 0; i < number_of_tests; i++)
        {
        int nRoots = SolveSquare (test[i][0], test[i][1], test[i][2], &x1, &x2);
        
        if (!((x1 == test[i][3] && x2 == test[i][4] && nRoots == 2) ||
              (x1 == test[i][4] && x2 == test[i][3] && nRoots == 2) ||
              (x1 == test[i][3] && nRoots == 1) ||
              (nRoots == test[i][4])))
            printf("test[%d]  wasn't passed\n", i);
        else printf("Ok\n");
        }
    }