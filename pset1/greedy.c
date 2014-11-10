#include <cs50.h>
#include <stdio.h>
#include <math.h>

/*
* Returns the total nubmber of US coins needed for $ value from the user.
* Uses a greedy algorithum to find the number of quarters, dimes, nickels, and pennies needed.
*/

int main(void)
{
    // Get amount in dollars from user
    float dollar_amount;
    do
    {
        printf("O hai!  How much change is owed?\n");
        dollar_amount = GetFloat();
    }
    while (dollar_amount < 0);
    
    // Change dollar amount (float) to cents (int)
    int cent_amount = round(dollar_amount * 100);
    
    // Calculate number of coins needed using mod and division.  Taking advantage of int's disregard of all info after the decimal
    int coins = 0;
    int coin_denominations[4] = {25, 10, 5, 1};
    
    // Runs calcuation for each of the four coin denominations
    for (int i = 0; i < 4; i++)
    {
        coins = coins + cent_amount/coin_denominations[i];
        cent_amount = cent_amount % coin_denominations[i];
    } 
    
    // Display number of coins
    printf("%d\n", coins);
    return 0;
}

