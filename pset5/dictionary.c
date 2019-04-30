/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

typedef struct trie_node
{
    char alphabet[27];
    struct trie_node* pointer[27];
    bool checker[27];
} node;

unsigned int sizer;
node* head;
void enter(node* crawler);
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int position;
    char word2;
    node* crawler = head;
    int n = strlen(word); //number of character in the word
    for (int i = 0; i < n; i++)
    {
        word2 = word[i];
        if (isalpha(word2))
        {
            if (isupper(word2))
                word2 = word2 + 'a' - 'A';
            position = (int)word2 - 97;
        }
        else
            position = (int)word2 - 13;
        if (i <= (n - 2))
        {
            if (crawler->pointer[position] == NULL)
                return false;
        }
        if (i == (n - 1))
        {
            if (crawler->checker[position])
                return true;
            else
                break;
        }
        crawler = crawler->pointer[position];
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    sizer = 0;
    head = malloc(sizeof(node));
    if (head == NULL)
    {
        printf("insufficient memory!\n");
        return false;
    }
    FILE* dc = fopen (dictionary, "r"); // open dictionary for reading
    if (dc == NULL)
    {
        printf("could not open the file\n");
        return false;
    }
    int d;
    char c;
    node* crawler;
    while ((c = fgetc(dc)) != EOF)
    {
        crawler = head;
        if (crawler == NULL)
        {
            printf("Error to asign crawler\n");
            return false;
        }
        sizer++;
        if (c == '\'')
            d = (int)c - 13;
        else
            d = (int)c - 97;
        head->alphabet[d] = c;
        while ((c = fgetc(dc)) != '\n' && c != EOF)
        {
            if (crawler->pointer[d] == NULL)
            {
                crawler->pointer[d] = malloc(sizeof(node));
                if (crawler->pointer[d] == NULL)
                {
                    printf("insufficient memory!\n");
                    return false;
                }
            }
            crawler = crawler->pointer[d];
            if (c == '\'')
                d = (int)c - 13;
            else
                d = (int)c - 97;
            crawler->alphabet[d] = c;
        }
        crawler->checker[d] = true;
    }
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return sizer;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    enter(head);
    return true;
}

void enter(node* crawler)
{
    int i = 0;
    while (true)
    {
        if (i == 27)
        {
            free(crawler);
            return;
        }
        if (crawler->pointer[i] != NULL)
            enter(crawler->pointer[i]);
        i++;
    }
}