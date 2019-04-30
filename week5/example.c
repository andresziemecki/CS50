#include <stdio.h>
#include <stdlib.h>
#include "single.h"

int main (void)
{
    int x = 5;
    insert_last(x);
    insert_last(x);
    insert_last(x);
    insert_last(x);
    insert_last(x);
    insert_position(1,4);
    insert_before_first(2);
    show_list();
    free_nodes();
    return 0;
    
}