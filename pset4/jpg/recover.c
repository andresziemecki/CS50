#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#define MAX_BYTE 512

typedef struct array_of_bytes
{
    uint8_t byte[MAX_BYTE];
} buffer;

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        printf("ERROR! Don't put any command-line!\n");
        return 1;
    }
    char* infile = "card.raw";
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("could not open the file\n");
        return 2;
    }
    int counter = 0;
    buffer jpg; //array of 512 bytes
    char* name = malloc(7*sizeof(char) + 1);
    if (name == NULL)
    return 3;
    
    
    while (!feof(inptr))
    {
        if (counter == 0)
            fread(&jpg, sizeof(buffer), 1, inptr);
        if (jpg.byte[0] == 0xff && jpg.byte[1] == 0xd8 && jpg.byte[2] == 0xff && jpg.byte[3] >= 0xe0 && jpg.byte[3] <= 0xef )
        {
            sprintf(name, "%03d.jpg", counter);
            FILE* outptr = fopen(name, "w");
            if (outptr == NULL)
                fclose(outptr);
            do
            {
                fwrite(&jpg, sizeof(buffer), 1, outptr);
                fread(&jpg, sizeof(buffer), 1, inptr);
                if (feof(inptr))
                {
                    break;
                }
            }
            while(jpg.byte[0] != 0xff || jpg.byte[1] != 0xd8 || jpg.byte[2] != 0xff || jpg.byte[3] < 0xe0);
            fclose(outptr);
            counter++;
        }
    }
    fclose(inptr);
    free(name);
    return 0;
}
