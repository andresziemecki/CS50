#include <stdio.h>
#include <cs50.h>
#include <math.h>

float insertchange(void);

int main (void)
{

    int coins;
    float change;
    coins = 0;
    change = insertchange();

    while (change>0.009)
    {
            if (change>=25)
    {
        coins = coins + (change-(int)change%25)/25; /* int because the operator % doesn´t work with float number */
        change = (int)change%25;
    }
    else if (change>=10 && change<25)
    {
        coins = coins + (change-(int)change%10)/10;
        change = (int)change%10;
    }
    else if (change>=5 && change<10)
    {
        coins = coins + (change-(int)change%5)/5;
        change = (int)change%5;
    }
    else
    {
        change = change-1;
        coins = coins+1;
    }
    }
    printf("%d\n", coins);
    return 0;
}
float insertchange()
{
    float change;
    do
    {
        printf("Insert the change: ");
        change = GetFloat();
    }
    
    while (change<0);
    change = change*10; /* this two line transofmr everything in cent */
    change = change*10; /* I multiplicated twice 10 and not 100 because with 100 It has a bug */
    return change;
}