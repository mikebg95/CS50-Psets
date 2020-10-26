#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // get dollar amount from user
    float dollars;
    do
    {
        dollars = get_float("Please insert the amount of change owed: ");
    }
    while (dollars <= 0);

    int cents = round(dollars * 100); // convert dollars to cents

    int coins = 0; // to keep track of number of coins

    while (cents >= 25)
    {
        cents -= 25;
        coins++;
    }
    while (cents >= 10)
    {
        cents -= 10;
        coins++;
    }
    while (cents >= 5)
    {
        cents -= 5;
        coins++;
    }
    while (cents >= 1) 
    {
        cents--;
        coins++;
    }

    printf("%i\n", coins); // print number of coins
}
