#ifndef SQUARE_EQUATION
#define SQUARE_EQUATION

#define number_of_tests 3

void read (double* a);
int Is_zero (double a);
int SolveSquare (double a, double b, double c, double* adr1, double* adr2);
int Solvelinear (double a, double b, double* adr1);
void Unit_tests();

#endif