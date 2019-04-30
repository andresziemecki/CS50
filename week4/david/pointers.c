#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main (void)
{
    printf("Say something: ");
    char* s = GetString();
    if (s == NULL) //es bueno evaluar esta condicion xq si es NULL podemos accidentalmente eliminar la referencia of that NULL pointer.
    {
        return 1;
    }
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        printf("%c\n", *(s+i)); // en s esta el primer caracter del string, en s+1 el segundo, y asi susesivamente.
    }
}