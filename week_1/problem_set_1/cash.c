#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // how much change is owed in dollar
    float change;
    do
    {
        change = get_float("How much change is owed?\n");
    }
    while (change < 0);

    // convert dollors into cents
    int cents = round(change * 100);

    // only coins available are
    // quarters (25), dimes (10), nickels (5), and pennies (1)
    int quaters = floor(cents / 25);
    int dimes = floor((cents - quaters * 25) / 10);
    int nickels = floor((cents - quaters * 25 - dimes * 10) / 5);
    int pennies = cents - quaters * 25 - dimes * 10 - nickels * 5;
    // print the total number of coins
    printf("%i\n", quaters + dimes + nickels + pennies);
}