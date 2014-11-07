#include <cs50.h>
#include <stdio.h>

/*
*Create a half pyramid, a-la Mario
*
*/

int main(void)
{
    // Get height from user
    int height;
    do
    {
        printf("Height: ");
        height = GetInt();
    }
    while (height < 0 || height > 23);
    
    // Exits when hight is 0
    if (height == 0)
    {
        return 0;
    }
    
    // Generate pyramid
    int space = height - 1;
    int hash = 2;
    while (height > 0)
    {
        
        // Print spaces
        for (int i = space; i > 0; i--)
        {
            printf(" ");
        }
        
        // Print hashes
        for (int j = hash; j > 0; j--)
        {
            printf("#");
        }
        
        // return to new line and incrament
        printf("\n");
        height = height - 1;
        space = space - 1;
        hash = hash + 1;       
    }
}
