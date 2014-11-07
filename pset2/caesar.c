#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * caesar.c
 *
 * Joshua Ferdaszewski
 * ferdaszewski@gmail.com
 *
 * Using a key and string from the user, the string is
 * encrypted using the caesar algorithem.
 */
 
int main(int argc, string argv[])
{
    // check for one and only one numberical command line argument
    if (argc != 2)
    {
        printf("Error: Enter one numeric key as argument\n");
        return 1;
    }
    int k = atoi(argv[1]);
    k = k % 26;
    
    // get a string from the user
    string p = GetString();
    
    // interate over each char in the string
    string c = p;
    for (int i = 0, n = strlen(p); i < n; i++)
    {
    
        // if the char is A-Z or a-z then move it k steps
        if (isalpha(p[i]) && isupper(p[i]))
            c[i] = ((p[i] - 'A' + k) % 26) + 'A';
        else if (isalpha(p[i]) && islower(p[i]))
            c[i] = ((p[i] - 'a' + k) % 26) + 'a';
        else
            c[i] = p[i];
    }
    
    // print cypher
    printf("%s\n", c);
    return 0;
 }
 
 
