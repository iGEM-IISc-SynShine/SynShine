#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double kd[2],K[3],alpha[2],beta[2],gamma[2],double b[2],double sigma[3],double Sigma[3], double delta[3], double Delta[3];

double logistic(double a, double b, double x) //Here this returns the value of a*x^2/(b + x^2)
{
	return (a*x*x)/(b + x*x);
}



int main(){

}