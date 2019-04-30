#include <cs50.h>
#include <stdio.h>
#include <string.h> //to compare the two string givin an adress (the function strcmp is in this library)

int main (void)
{
    printf("say something: ");
    char* s = GetString();
    printf("say something: ");
    char* t = GetString();
    if (s != NULL && t != NULL) /* hay que saber que string a veces returna NULL (algo malo paso), o xq nos qdamos sin memoria o el usuario no coopero, etc.
                                   si yo paso algo NULL, cosas malas pueden pasar, el programa puede "explotar" por ejemplo. Pero generlamente 
                                   lo devuelve como "segmentation faults */
    {
        if (strcmp(s, t) == 0)
            printf("you typed the same thing!\n");
        else
            printf("Yo typed differen things!\n");
        return 0;
    }
}