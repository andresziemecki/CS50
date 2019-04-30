#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_poiner
{
    int* pointers[10];
} node;

int main (void)
{
    node* ptr = malloc(sizeof(node));
    for (int i = 0; i < 10; i++)
    {
        ptr->pointers[i] = malloc(sizeof(int));
        *ptr->pointers[i] = i;
    }
    free(ptr->pointers);
    free(ptr);
    return 0;
}

