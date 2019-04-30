//mete las salidas del pograma en un archivo
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //tiene algunas funciones relacionadas con os archivos I/O (input y output)

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
    
    FILE* file = fopen("students.csv", "w"); //w: is because the program must open this file for Writting, si existe el archivo se reemplaza con el nuevo
    if (file != NULL) // porque si es probablemente haya algo que este llendo mal, y no tendriamos que proceder a intentar printf algo
    {
        for (int i = 0; i < NUMBER; i++)
        {
            fprintf(file, "%s, %s\n", students[i].name, students[i].house);
        }
        fclose(file); // tambien existe fopen(file)
    }
    
    for (int j = 0; j < NUMBER; j++)
    {
        free(students[j].name);
        free(students[j].house);
    }
    return 0;
}