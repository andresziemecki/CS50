#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int GetPositiveInt(void);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        int key = atoi(argv[1]); //transform the char into an int
        
        if (key < 0)
        {
            key = GetPositiveInt(); //eveluates the key if it is a positive int
        }
        string s = GetString(); //input the string to encript
        
        for (int i = 0, n = strlen(s); i < n; i++) //code to encrypt the message
        {
            if (isupper(s[i]))
            {
                s[i] = (s[i]%65 + key)%26 + 65;
                printf("%c", s[i]);
            }
            else if (islower(s[i]))
            {
                s[i] = (s[i]%97 + key)%26 + 97;
                printf("%c", s[i]);
            }
            else
                printf("%c", s[i]);
        }
        printf("\n");
        return 0;
    }
    else
    {
        printf("ERROR! You haven't put any key!");
        return 1;
    }
}

int GetPositiveInt(void)
{
    int number;
    do
    {
        printf("Enter a Positive Number: ");
        number = GetInt();
    }
    while (number < 0);
    return number;
}