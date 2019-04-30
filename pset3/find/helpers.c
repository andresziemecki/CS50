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
{ // TODO: implement a searching algorithm
    int midle;
    int min = 0; 
    int max = n - 1;
    int counter = 1;
    if (n<0)
        return false;
    else
    {
        while (counter>0)
        {
            midle = (max + min)/2;
            printf("%i\t %i\t %i\n", min, midle, max);
            if (value == values[midle])
                return true;
            else if (min == max)
                counter = 0;
            else if (value < values[midle])
                max = (midle - 1);
            else if (value > values[midle])
                min = (midle + 1);
            
        }
    }
    return false;
}
    
    /* LINEAR SEARCH:
    if (n < 0)
        return false;
    else
        for (int i = 0; i<n; i++)
        {
            if (value == values[i])
            return true;
        }
        return false;
        */

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int min, counter = 1;
    while (counter != 0)
    {
        counter = 0;
        for (int i=0; i<(n - 1); i++)
        {
            if (values[i]>values[i + 1])
            {
                min = values[i + 1];
                values[i+1] = values[i];
                values[i] = min;
                counter = 1;
            }
        }
    }    
    return;
}