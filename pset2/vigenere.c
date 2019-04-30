#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc == 2) //if there are only 2 commands line, this code it will work.
    {
        int n = strlen(argv[1]); //I have to know how many characters are in the secret key to cointinue
        for (int i = 0; i<n; i++) // this for is going to check if the characters are alphabetical
        {
            if (isalpha(argv[1][i])) //if is alphabetical each letter must be in his corresponde number of the normal alphabet
            {
                if (isupper(argv[1][i]))
                    argv[1][i] = argv[1][i] % 65;
                else
                    argv[1][i] = argv[1][i] % 97;
            }
            else //if there some character that is not avaliable, the program will end 
            {
                printf("You put an invalid key!");
                return 1;
            }
        }
        string s = GetString(); //now the input of the message that you want to encript
        for (int k = 0, j = 0, m = strlen(s); j<m; j++) //the formula to encrypt is this one
        {
            if (isupper(s[j]))
            {
                s[j] = (s[j]%65 + argv[1][k%n])%26 + 65;
                k++;
            }
            else if (islower(s[j]))
            {
                s[j] = (s[j]%97 + argv[1][k%n])%26 + 97;
                k++;
            }
        }
        printf("%s\n", s); //output the message encrypted
        return 0;
    }
    else //if there are one or more than 2 command-line the program will not run
    {
        printf("You put an invalid key!");
        return 1;
    }
}
   