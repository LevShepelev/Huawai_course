#include <stdio.h>
#include <math.h>

// разнести функционал по файлам + написать юниты + доки

const double epsilon = 1e-7;
void red (double* a);
int comparing (double a);
int SolveSquare (double a, double b, double c, double* adr1, double* adr2);
void Solvelinear (double a, double b, double* adr1);

//-----------------------------------------------------------------------------

void red (double* a) // The function read double number symbol by symbol, comma input supported
    {
    int i = 0, err = 0, aftpoint = 0, n_aft = 0, nscan = 0, npoint = 0, neg = 0;

    char num[40] = {'\0'};
    do               // Checking correctness of input
        {
        err = 0;
        // почему нельзя было использовать сразу %f?
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
        }
    while (err == 1);



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


//-----------------------------------------------------------------------------

/*
bool is_zero(double num) 
    {
       return fabs (num) < epsilon;
    }
*/
int comparing (double a)  // Function compare double number with zero
    {
    //сделать тернарный оператор
    if (a > epsilon) return 1;
    if (fabs (a) < epsilon) return 0;
    if (a < -epsilon) return -1;
    return 0;
    }

//-----------------------------------------------------------------------------

void Solvelinear (double b, double c, double* adr1) // Function solves linear equation
    {
    //  проверка на нулевой указатель через assert
    printf("Your equation if linear, ");

    if ((comparing (b) != 0) && (comparing (c) == 0))
            {
            printf ("1 root, x = %d\n", 0);
            *adr1 = 0;
            }

    if (((int)fabs(comparing (b)) == 1) && (comparing (c) != 0))
        {
        printf ("1 root, x = %lg\n", -c / b);
        *adr1 = -c / b;
        }

    if ((comparing (b) == 0) && (comparing (c) == 0))
        printf ("Infinite number of roots");

    if (((comparing (b) == 0) && ((int)fabs(comparing (c)) == 1)))
        printf ("No roots");
    }

//-----------------------------------------------------------------------------

int SolveSquare (double a, double b, double c, double* adr1, double* adr2)   // Function Soleves Square Equation
    {
    // зачем так рано считать дискриминант?
    double discriminant = b*b - 4*a*c;
    if (fabs (a) > epsilon)
        {
        if (comparing (discriminant) == 1)
            {
            *adr1 = (-b - sqrt (discriminant)) / (2*a);
            *adr2 = (-b + sqrt (discriminant)) / (2*a);
            return 2;
            }
        if (comparing (discriminant) == 0)
            {
            *adr1 = -b / (2*a);
            return 1;
            }
        if (comparing (discriminant) == -1)
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


int main()
    {
    printf ("Square Equation Solver 2020\nWrite coefficient a, b, c\n");
    double mass[3];
    double x1 = 0, x2 = 0;
    for (int i = 0; i < 3; i++)
        red (mass + i);
    int nRoots = SolveSquare (mass[0], mass[1], mass[2], &x1, &x2);
    if (nRoots == 2)
        {
        printf ("x1 = %+lg\n"  "x2 = %+lg\n", x1, x2);
        }
    if  (nRoots == 1)
        {
        printf ("x = %lg\n", x1) ;
        }
    return 0;
    }





