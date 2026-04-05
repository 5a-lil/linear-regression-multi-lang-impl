
#include <stdio.h>

#define I 0
#define J 1
#define M 24
#define ITERS 1000

int values[M][2] = {
    { 240000, 3650 },
    { 139800, 3800 },
    { 150500, 4400 },
    { 185530, 4450 },
    { 176000, 5250 },
    { 114800, 5350 },
    { 166800, 5800 },
    { 89000, 5990 },
    { 144500, 5999 },
    { 84000, 6200 },
    { 82029, 6390 },
    { 63060, 6390 },
    { 74000, 6600 },
    { 97500, 6800 },
    { 67000, 6800 },
    { 76025, 6900 },
    { 48235, 6900 },
    { 93000, 6990 },
    { 60949, 7490 },
    { 65674, 7555 },
    { 54000, 7990 },
    { 68500, 7990 },
    { 22899, 7990 },
    { 61789, 8290 }
};
double theta0 = 0;
double theta1 = 0;
double learn_rate = 0.1;

double estimated_price(double x)
{
    return theta0 + theta1 * x;
}

void calc_new_thetas()
{
    double sum_estimated_prices_theta0 = 0;
    for (size_t i = 0; i < M; i++) {
        sum_estimated_prices_theta0 += estimated_price(values[i][I]) - values[i][J];
    }

    double sum_estimated_prices_theta1 = 0;
    for (size_t i = 0; i < M; i++) {
        sum_estimated_prices_theta1 += (estimated_price(values[i][I]) - values[i][J]) * values[i][I];
    }
    // ---------
    double theta0_var = learn_rate * (sum_estimated_prices_theta0 / M);
    double theta1_var = learn_rate * (sum_estimated_prices_theta1 / M);
    theta0 -= theta0_var;
    theta1 -= theta1_var;
}

int main()
{
    for (int i1 = 0; i1 < ITERS; i1++)
    {
        calc_new_thetas();
        printf("theta0 = %f\ntheta1 = %f\n-------\n", theta0, theta1);
    }
}
