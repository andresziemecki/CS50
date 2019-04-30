#include <stdio.h>
#include <stdlib.h>

#include "single.h"

// define struct node
typedef struct node
{
    int value;
    struct node* next;
} node;

node* head = NULL;


void insert_last (int insert)
{
    // check if there isn't a first value
    if (head == NULL)
    {
        // if there isn't a first value put the insert here
        head = malloc(sizeof(node));
        if (head == NULL)
        {
            printf("Run out of memory!\n");
            return;
        }
        else
        {
            head -> value = insert;
            head -> next = NULL;
        }
        printf("Succesful insert!\n");
    }
    
    // if there is a first value, check till the end of the node
    else
    {
        node* crawler = head;
        if (crawler == NULL)
        {
            printf("Something bad happend\n");
            return;
        }
        while (1)
        {
            if (crawler -> next == NULL)
                break;
            else
                crawler = crawler -> next;
        }
        
        // put the new value in the end of the node
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
            {
                printf("Run out of memory!\n");
                return ;
            }
        crawler -> next = new_node;
        new_node -> value = insert;
        new_node -> next = NULL;
        printf("Succesful insert!\n");
    }
    return;
}

void insert_position (int position, int insert)
{
    //check if the input was the first position
    if (position == 0)
    {
        printf("It can't be the first position\n");
        return;
    }
    
    //checks for any node created before
    if (head == NULL)
    {
        printf("There is nothing in the node\n");
        return;
    }
    
    // go to the position
    node* crawler = head;
    if (crawler == NULL)
    {
        printf("Asignament pointer fail\n");
        return;
    }
    
    for (int i = 0; i < position - 1; i++)
    
    {
        crawler = crawler -> next;
        if (crawler == NULL)
        {
            printf("No llega a esa posicion\n");
            return;
        }
    }
    
    // inert the new node
    node* new_node = malloc(sizeof(node));
    if (new_node == NULL)
    {
        printf("insufficient memory\n");
        return;
    }
    
    new_node -> value = insert;
    new_node -> next = crawler -> next;
    crawler -> next = new_node;
    printf("Succesful insert!\n");
    
    return;
}

void show_list (void)
{
    //checks for any node created before
    if (head == NULL)
    {
        printf("There is nothing in the node\n");
        return;
    }
    
    // check if something bad happend in the asignament pointer
    node* crawler = head;
    if (crawler == NULL)
    {
        printf("There isn't any node\n");
        return;
    }
    
    // print all the array
    while (1)
    {
        printf("%i\n", crawler->value);
        if (crawler -> next == NULL)
            break;
        else
            crawler = crawler -> next;
    }
    
    printf("show_list Succesful!\n");
    
    return;
}

void free_nodes (void)
{
    // check if there is any node in the heap
    if (head == NULL)
    {
        printf("there isn't any node\n");
        return;
    }
    
    node* crawler = NULL;
    
    // free all of them
    do
    {
        crawler = head;
        while (crawler != NULL)
        {
            crawler = crawler-> next;
        }
        free(crawler);
    }
    while (head == NULL);
    
    printf("Everything is free!\n");
    
    return;
}

void insert_before_first(int insert)
{
    // check if there exist any node
    if (head == NULL)
    {
        printf("There isn't any node created\n");
        return;
    }
    
    //define the first node
    node* new_node = malloc(sizeof(node));
    if (new_node == NULL)
    {
        printf("There is not enough memory!\n");
        return;
    }
    
    //insert the first node
    new_node -> value = insert;
    new_node -> next = head;
    head = new_node;
    printf("Succesful first node inserted!\n");
    return;
}