#include <cs50.h>
#include <stdio.h>
#include <string.h>

#include "structs.h"

#define NUMBER 2

int main (void)
{
    student students[NUMBER];
    for (int i = 0; i < NUMBER; i++)
    {
        printf("nstudent's name: ");
        students[i].name = GetString();
        printf("student's house: ");
        students[i].house = GetString();
    }
    for (int j = 0; j < NUMBER; j++)
    {
        printf("%s is in %s.\n", students[j].name, students[j].house);
        free(students[j].name);
        free(students[j].house);
    }
}