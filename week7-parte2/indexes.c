#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

char* indexes(const char* path);
typedef char BYTE;
int main(void)
{
    const char* path = "z";
    char* probando = indexes(path);
    printf("%s\n", probando);
    return 0;
    
}

char* indexes(const char* path)
{
    // count how many characters has directory
    int i = strlen(path);
    
    //create a variable to store directory and index.html
    char* index1 = malloc(i*sizeof(BYTE) + 10);
    if (index1 == NULL)
        return NULL;
    
    // store path in index1
    strcpy(index1, path);
    
    // append to index1 and put index.php
    strcat(index1, "/index.php");
    
    // try to open this file
    FILE* fp = NULL;
    fp = fopen(index1, "r");
    if (fp != NULL)
        {
            fclose(fp);
            return index1;
        }
    
    // if doesn't result, free the memory and proced the same with the other file
    free(index1);
    char* index2 = malloc(i*sizeof(BYTE) + 11);
    if (index2 == NULL)
        return NULL;
    strcpy(index2, path);
    strcat(index2, "/index.html");
    fp = fopen(index2, "r");
    if (fp != NULL)
    {
        fclose(fp);
        return index2;
    }
    
    // if no-one of the file could open, free and terminate
    free(index2);
    return NULL;
}
