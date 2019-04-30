/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h> //the library that include srand and rand
#include <time.h> 

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // if you input less than 2 or more than 3 line arguments, the program it will not work.
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // atoi() convert char numbers into an int numbers
    int n = atoi(argv[1]);

    /* if we give it an third argument, the code of "if" is going to convert the char caracter into an int
    and execute srand48 that execute the "seed" of the random number generate in drand48 with 48bits, if we don't give it a third command line argument
    the code is going to generate a "seed" that depends the time that we are executing the code, time(NULL) is including in the library of <time.h> */
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        srand48((long int) time(NULL));
    }

    /* this for is going to give us an output of n numbers that depends of the second command-line argument
    The drand48() functions return nonnegative double-precision floating-point values uniformly distributed between [0.0, 1.0)
    and them multiply with the constant "limit", the random numbers generated depends the "seed" that you give it from srand */ 
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}