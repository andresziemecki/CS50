#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int val;
    struct* node next;
} node;

node* head = NULL;

void insert_after_third (int to_insert)
{
    //if the list is empty
    if (head == NULL)
    {
        printf("List empty. No thid positio exist\n");
    }
    else
    {
        //traversal pointer
        node* crawler = head;
        
        // set crawler to the third node in the list
        for (int i = 0; i < 3; i++)
        {
            //if already at the end of the list, return
            if (crawler->next == NULL)
            {
                printf("List not long enough. No 3rd position exist.\n");
                return;
            }
            
            // otherwise keep traversing the list
            else
            crawler = crawler -> next;
        }
        
        //initialize a new node to go between the two
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Out of heap memory!\n");
            return;
        }
        
        //plop in the ingtegrer value
        new_node -> value = to_insert;
        
        // first point our new node's "next" field at the third node's successor
        new_node -> next = crawler -> next;
        
        // then we can point our previous node's "next" field at the new one
        crawler -> next = new_node
    }
}

void print_last ()
{
    if (head == NULL)
    {
        printf("List empty. No last element\n");
    }
    
    else
    {
        // traversal pointer
        node* crawler = head;
        
        //until the end of the list
        while (crawler->next != NULL) // (*crawler).next
        {
            //keep traversing the list
            crawler = crawler -> next;
        }
        //print the last integrer in the list
        printf("The last integrer is: %d\n", crawler->val);
        
    }
}