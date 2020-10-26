// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int hash_number = hash(word);
    node *cursor = table[hash_number];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
// SOURCE: https://github.com/edouardjamin/CS50/blob/master/pset5/dictionary.c
unsigned int hash(const char *word)
{
    int total = 0;

    // add ascii values of chars in word together
    for (int i = 0; word[i] != '\0'; i++)
    {
        total += tolower(word[i]);
    }

    return total / N;
}

// Loads dictionary into memory, returning true if successful else false
// ***LEAKS MEMORY: NOT POSSIBLE TO FREE NODE N***
bool load(const char *dictionary)
{
    // open dictionary file and check for null
    FILE *infile = fopen(dictionary, "r");
    if (infile == NULL)
    {
        printf("Error: can't open dictionary\n");
        return false;
    }

    char word[LENGTH]; // variable to store word

    node *n = NULL;

    while (fscanf(infile, "%s", word) != EOF)
    {
        // allocate memory for and create node; check for null
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Error: memory could not be allocated\n");
        }

        // copy word into node (works)
        strcpy(n->word, word);
        n->next = NULL;

        // get hash number for word from hash function
        int hash_number = hash(word);

        // if it's first word
        if (table[hash_number] == NULL)
        {
            table[hash_number] = n;
        }

        // if it's not first word
        else
        {
            n->next = table[hash_number]->next;
            table[hash_number]->next = n;
        }
        word_count++;
    }

    fclose(infile);
    // free(n); // ***THIS LEADS TO ERRORS IN PROGRAM***

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    //  for (int i = 0; i < N; i++)
    // {
    //     // initiate a cursor
    //     node *cursor;

    //     // place the cursor inside the hashtable
    //     cursor = table[i];

    //     while (cursor)
    //     {
    //         node* tmp = cursor;
    //         cursor = cursor->next;
    //         free(tmp);
    //         return true;
    //     }

    //     // clean the hashtable
    //     table[i] = NULL;
    // }

    // return false;

    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;

        }
        free(cursor);
        free(tmp);
    }
    return true;
}
