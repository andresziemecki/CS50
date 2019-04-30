#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs50.h>

typedef char BYTE;

bool load(FILE* file, BYTE** content, size_t* length);

int main (void)
{
    FILE* file = fopen("z/index.php", "r");
    if (file == NULL)
    {
        return 1;
    }
    BYTE* content = NULL;
    size_t length;
    if (load(file, &content, &length))
    {
        printf("%lu\n", length);
        return 0;
    }
    return 1;
}



bool load(FILE* file, BYTE** content, size_t* length)
{
    if (file==NULL) { return false; }

    // obtain file size:
    fseek (file , 0 , SEEK_END);
    long lSize;
    lSize = ftell (file);
    rewind (file);

    // allocate memory to contain the whole file:
    char* buffer = malloc(sizeof(char)*lSize + 1);
    if (buffer == NULL) 
        return false; 

    // copy the file into the buffer and put the size in length
    *length = fread (buffer,1,lSize,file);

    // the whole file is now loaded in the memory buffer

    // terminate
    *content = buffer;
    return true;
}