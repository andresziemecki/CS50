#include <stdio.h>
#include <cs50.h>

int getpositiveint(void);

int main(void)
{
    int height, column, maxcolumn;
    maxcolumn = 1 + getpositiveint(); //the height is one number less than maxcolumn
    for (height = maxcolumn-1; height >= 1; height--) //this for is for heach floor of phyramid
    {
        for(column = 1; column <= maxcolumn; column++) //this for is for each column on specific floor
        {
            if (height - column > 0)
            printf(" ");
            else
            printf("#");
        }
        printf("\n");
    }
    return 0;
}

int getpositiveint(void)
{
    int n;
    do
    {
        printf("How much hight do you want the phyramid of Mario? max(23)\n");
        n = GetInt();
    }
    while (n<0 || n>23);
    return n;
}



            

