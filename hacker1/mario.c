#include <stdio.h>
#include <cs50.h>

int GivePositiveInt(void);

int main (void)
{
    int i, j, Height = GivePositiveInt();
    
    for (int n = 1; n <= Height; n++)
    {
        for (i = 0; i < Height - n; i++)
            printf(" ");
        for (j = Height - i; j > 0; j--)
            printf("#");
        printf("  ");
        for(j = Height - i; j > 0; j--)
            printf("#");
        printf("\n");
    }
    return 0;
}

int GivePositiveInt(void)
{
    int Height;
    do
    {
        printf("Hight: ");
        Height = GetInt();
    }
    while(Height < 0 || Height > 23);
    return Height;
}