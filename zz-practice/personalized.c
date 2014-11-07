#include <cs50.h>
#include <stdio.h>

/**
 * personalized.c
 *
 * Joshua Ferdaszewski
 * ferdaszewski@gmail.com
 *
 * Takes users full name as a command line argument and prints
 * a greating to the user that includes their first name.
 */
 
int main(int argc, string argv[])
{
    if (argc != 3)
    {
        return 1;
    }
    printf("Ohai %s\n", argv[1]);
}
