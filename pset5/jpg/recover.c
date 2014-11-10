/**
 * Joshua Ferdaszewski
 * ferdaszewski@gmail.com
 *
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image passed in as a command line argument.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// 512 size bock of FAT file system
typedef uint8_t  BYTE;
typedef struct
{
    BYTE head[4];
    BYTE body[508];
}BLOCK;

int main(void)
{
    // open data file 
    FILE* dataptr = fopen("card.raw", "r");
    if (dataptr == NULL)
    {
        printf("Could not open card.raw\n");
        return 1;
    }
    
    // pointer to hold block of data
    BLOCK* buffer = malloc(sizeof(BLOCK));
    
    // pointer for jpg file
    FILE* jpgptr = NULL;
    
    // variable to track number of jpgs found
    int jpgnum = 0;
    
    // repeat untill end of card
    while (true)
    {
        // read block - if EOF stop and close final jpg file
        if (fread(buffer, sizeof(BLOCK), 1, dataptr) != 1)
        {
            // close jpg file
            fclose(jpgptr);
            
            break;
        }
        
        // check for jpg at start of head
        if (buffer->head[0] == 0xff && buffer->head[1] == 0xd8 && buffer->head[2] == 0xff && (buffer->head[3] == 0xe0 || buffer->head[3] == 0xe1))
        {
            // if not first jpg found
            if (jpgnum > 0)
            {
                // close jpg file
                fclose(jpgptr);
            }
            
            // create new jpg file
            char title[10];
            sprintf(title, "%.3d.jpg", jpgnum);
            
            // open new jpg file
            jpgptr = fopen(title, "a");
            
            // write block to jpg file
            fwrite(buffer, sizeof(BLOCK), 1, jpgptr);
            
            // incremint the number of jpgs found
            jpgnum++;
        }
        
        // no jpg header found in block
        else
        {        
            // if jpg file is open
            if (jpgnum > 0)
            {
                // write block to jpg file
                fwrite(buffer, sizeof(BLOCK), 1, jpgptr);
            }
        }
    }
    
    // release memory
    free(buffer);
    
    return 0;
}
