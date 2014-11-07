#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * vigenere.c
 *
 * Joshua Ferdaszewski
 * ferdaszewski@gmail.com
 *
 * Using a keyword and string from the user, the string is
 * encrypted using the vigenere cipher.
 */
 
int main(int argc, string argv[])
{
    // check for one and only one string command line argument
    if (argc != 2)
    {
        printf("Error: Enter one keyword as argument\n");
        return 1;
    }
    
    // initilize key using keyword
    int key[strlen(argv[1])];
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isalpha(argv[1][i]) && isupper(argv[1][i]))
            key[i] = argv[1][i] - 'A';
        else if (isalpha(argv[1][i]) && islower(argv[1][i]))
            key[i] = argv[1][i] - 'a';
        else
        {
            printf("Error: Keyword has non-alpha char\n");
            return 1;
        }
    }
    
    // get a string from the user
    string p = GetString();
    
    // interate over each char in the string
    for (int i = 0, n = strlen(p), j = 0; i < n; i++)
    {
    
        // if the char is A-Z or a-z then move it k[j] steps
        if (isalpha(p[i]) && isupper(p[i]))
        {
            p[i] = ((p[i] - 'A' + key[j % strlen(argv[1])]) % 26) + 'A';
            j++;
        }
        else if (isalpha(p[i]) && islower(p[i]))
        {
            p[i] = ((p[i] - 'a' + key[j % strlen(argv[1])]) % 26) + 'a';
            j++;
        }
        else
            p[i] = p[i];
    }
    
    // print cypher
    printf("%s\n", p);
    return 0;
 }
 
 
