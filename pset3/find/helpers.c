/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // binary search
    if (n <= 0)
    {
        return false;
    }
    int mid, min, max;
    min = 0;
    max = n -1;
    mid = (max + min) / 2;
    while (min <= max)
    {
        if (values[mid] == value)
        {
            return true;
        }
        else if (values[mid] > value)
        {
            max = mid - 1;
        }
        else
        {
            min = mid + 1;
        }
        mid = (max + min) / 2;
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // insertion sort
    if (n <= 0)
    {
        return;
    }
    int x;
    for (int i = 0; i < n; i++)
    {
        x = values[i];
        int j;
        for (j = i; j > 0 && values[j-1] > x; j--)
        {
            values[j] = values[j-1];
        }
        values [j] = x;
    }
    return;
}
