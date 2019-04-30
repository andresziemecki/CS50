#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
    printf("Say something: ");
    char* s = GetString();
    if (s == NULL) //es bueno evaluar esta condicion xq si es NULL podemos accidentalmente eliminar la referencia of that NULL pointer.
    {
        return 1;
    }
    string t = malloc( (strlen(s) + 1)*sizeof(char)); // memory allocation, return an adress for a chunk of memory of that many bytes
    //si malloc falla en darnos la memoria que necesitamos es mejor chequear eso antes d mandarnos un pedo
    if (t == NULL)
    {
        free(s);
        return 1;
    }
    for(int i = 0, n = strlen(s); i <= n /* <= porqueincluyo el caracter terminal nulo */; i++)
    {
        // t[i] = s[i]; esta es una opcion para copiar, y la siguiente es otra: 
        *(t + i) = *(s + i);
    }
    printf("Capitalizing the copy...\n");
    if (strlen(t) > 0)
    {
        t[0] = toupper(t[0]);
    }
    printf("Original value is: %s\n", s);
    printf("Value of the Copy: %s\n", t);
    return 0;
}