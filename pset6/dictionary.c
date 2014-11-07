/****************************************************************************
 * dictionary.c
 * 
 * Joshua Ferdaszewski
 * ferdaszewsi@gmail.com
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// root node of tries data structure
struct node* root;

// variable to track the number of words loaded into dictionary
unsigned int words = 0;

/**
 * alocates and returns a new node in the tries data structure
 */
struct node* nalloc(void)
{
    // allocate a node in memory inilitalised to 0
    struct node* newnode = calloc(1, sizeof(node));

    // set isword to false
    newnode->isword = false;
    
    // return the new node
    return newnode;
}

/**
 * Recersive function to travel to bottom of tries and free each
 * node that has all NULL children pointers.  Returns true if
 * sucsessful else false.
 */
bool freenode(struct node* q)
{
    // interate through children in node
    for (int i = 0; i < SIZE; i++)
    {
        // if the current child is NULL
        if (q->children[i] != NULL)
        {
            // call freenode on the next node down
            freenode(q->children[i]);
        }
    }
    
    // free this empty node
    free (q);
    
    // done!
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // pointer to hold the current node while iterating throught the tries, starting at root
    struct node* q = root;
    
    // variable to hold the node index of char
    int i;
    
    // iterate through word char by char untill NULL is reached
    for (int j = 0; word[j] != '\0'; j++)
    {
        // convert char to node index
        if (isalpha(word[j]))
        {
            // convert lowercase alpha to index (a or A=0, b or B=1 ... z or Z=25)
            i = tolower(word[j]) - 'a';
        }
        
        // if apostrophy
        else if ( word[j] == '\'')
        {
            // convert to last index number
            i = SIZE - 1;
        }
        
        // check for NULL at current node
        if (q->children[i] == NULL)
        {
            // word not found
            return false;           
        }
        
        // point q to next node
        q = q->children[i];
    }
    
    // all chars in tries, final check for isword
    return q->isword;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 * assume lexicographically sorted from top to bottom with one word per line,
 * each of which ends with \n. No word will be longer than LENGTH characters, 
 * that no word will appear more than once, and that each word will contain
 * only lowercase alphabetical characters and possibly apostrophes.
 */
bool load(const char* dictionary)
{    
    // open dictionary file
    FILE* dptr = fopen(dictionary, "r");
    if (dptr == NULL)
    {
        printf("Could not open dictionary file: %s\n", dictionary);
        return false;
    }
    
    // intialize root node
    root = nalloc();
    
    // variable to hold the node index of char
    int i;
    
    // pointer to hold the current node while iterating throught the tries, starting at root
    struct node* q = root;
    
    // read file char by char untill EOF is reached
    for(int c = fgetc(dptr); c != EOF; c = fgetc(dptr))
    {
        // check for end of line/word
        if (c == '\n')
        {
            // incrimint word count
            words++;
            
            // check for NULL at current node
            if (q->children[i] == NULL)
            {
                // create new node and point q to it
                q->children[i] = nalloc();            
            }
            
            // set isword to true
            q->isword = true;
            
            // reset for next word
            q = root;
            continue;
        }
        
        // convert char to node index
        else if (isalpha(c))
        {
            // convert alpha to index (a=0, b=1 ... z=25)
            i = c - 'a';
        }
        
        // must be apostrophy
        else
        {
            // convert to last index number
            i = SIZE - 1;
        }
             
        // check for NULL at current node
        if (q->children[i] == NULL)
        {
            // create new node and point q to it
            q->children[i] = nalloc();            
        }
       
        // point q to next node
        q = q->children[i];
    }
    
    // close dictionary file
    fclose(dptr);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // pass the word count variable counted on dictionary load
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // start recursive freenode function on root
    bool memoryfree = freenode(root);
    
    // return answer
    return memoryfree;
}
