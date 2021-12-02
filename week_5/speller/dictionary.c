// Implements a dictionary's functionality
// files other than this are provided by the project

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"
int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
// must be case insensitive
bool check(const char *word)
{
    // hash word to obtain a hash value
    int index = hash(word);
    // access linkedlist at the index in the hash table
    // start with cursor set to first item in the linkedlist
    node *cursor = table[index];
    // traverse linkedlist, looking for the word
    // keep moving cursor until getting NULL
    while (cursor != NULL)
    {
        // checking each node for the word
        if (strcasecmp(word, cursor -> word) == 0)
        {
            return true;
        }
        cursor = cursor -> next;
    }
    // no match is found throughout the linkedlist
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // index: 0 - 675 (26*26-1)
    char first_letter = tolower(word[0]);
    char second_letter;

    // if word length = 1, put it with xa
    if (strlen(word) == 1)
    {
        second_letter = 'a';
    }
    // if the second letter is ', put it with xa
    else if (word[1] == '\'')
    {
        second_letter = 'a';
    }
    // look at the first 2 letters of the word: xx
    else
    {
        second_letter = tolower(word[1]);
    }

    int first_letter_num = (int) first_letter - 97;
    int second_letter_num = (int) second_letter - 97;

    return first_letter_num * 26 + second_letter;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // STEP 1: open dictionary file
    FILE *dict = fopen(dictionary, "r");
    // check if the return value is null
    if (dict == NULL)
    {
        return false;
    }

    // STEP 2: read strings from file one at a time
    // word is a char array with allocated memory
    char word[LENGTH + 1];
    // read the chars into word and access later
    // fscanf will return EOF once it reaches end of file
    while (fscanf(dict, "%s", word) != EOF)
    {
        // create a new node for each word
        node *n = malloc(sizeof(node));
        // use malloc, check if return value is null
        if (n == NULL)
        {
            return false;
        }
        // copy word into node using strcpy
        strcpy(n -> word, word);
        n -> next = NULL;

        // hash word to obtain a hash value
        // use the hash function above - takes a string, returns an index
        // index: indicate which linked list this word will be in
        int index = hash(word);

        // insert node into hash table at that location
        // index into the hash table to get the linked list wanted
        // create & insert a new node add this word into

        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            // first point the new node to the original head
            n -> next = table[index] -> next;
            // then point the index to the new node
            table[index] -> next = n;
        }

        // keep track of the number of words in the hash table
        word_count++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor;
    node *tmp;
    // free each of the linked list in the hash table
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor -> next;
            free(tmp);
        }
    }
    return true;
}
