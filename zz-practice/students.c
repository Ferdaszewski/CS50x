#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * students.c
 *
 * Joshua Ferdaszewski
 * ferdaszewski@gmai.com
 *
 * Gets 5 names from user and randomly prints one of the names.
 */

int main(void)
{
    // get 5 names from user
    string names[5];
    for (int i = 0; i < 5; i++)
    {
            printf("Give me name %d: ", i+1);
            names[i] = GetString();
    }
    
    // randomly select one name
    srand(time(NULL));
    int random = rand() % 5;
    
    // print name
    printf("%s\n", names[random]);
}
