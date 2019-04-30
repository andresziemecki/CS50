#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main (void)
{
    string s = GetString();
    
        if (s[0] >= 'a' && s[0] <= 'z') // I had to change the first apart from others because I put has condition: ' ' (space)
        {
            s[0] = s[0] - ('a' - 'A');
        }
        
        printf("%c", s[0]);
        
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ' ')
        {
            if (s[i+1] >= 'a' && s[i+1] <= 'z')
            {
                s[i+1] = s[i+1] - ('a' - 'A');
            }
            printf("%c", s[i+1]);
        }
    }
    printf("\n");
}

