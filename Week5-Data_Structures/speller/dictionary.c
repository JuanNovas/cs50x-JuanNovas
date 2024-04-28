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
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

int n_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int buck = hash(word);
    node *buffer = table[buck];
    while (buffer != NULL)
    {
        if (!strcasecmp(word, buffer->word))
        {
            return true;
        }
        else if (buffer->next != NULL)
        {
            buffer = buffer->next;
        }
        else
        {
            break;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    if (strlen(word) == 1)
    {
        return toupper(word[0]) - 'A';
    }
    int a = 0;
    int b = 0;
    a = (toupper(word[0]) - 'A') * 26;
    b = toupper(word[1]) - 'A';
    return a + b;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fclose(file);
        unload();
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        n_words++;
        node *aaa = malloc(sizeof(node));
        if (aaa == NULL)
        {
            fclose(file);
            unload();
            return false;
        }
        strcpy(aaa->word, word);
        int number = hash(aaa->word);
        if (table[number] == NULL)
        {
            table[number] = aaa;
            table[number]->next = NULL;
        }
        else if (table[number]->next == NULL)
        {
            aaa->next = NULL;
            table[number]->next = aaa;
        }
        else
        {
            aaa->next = table[number]->next;
            table[number]->next = aaa;
        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return n_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        node *tmp2;
        while (tmp != NULL)
        {
            tmp2 = tmp->next;
            free(tmp);
            tmp = tmp2;
        }
    }
    return true;
}
