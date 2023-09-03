#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in the hash table
#define N 100000

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Convert word to lowercase
    char lowercase[LENGTH + 1];
    int length = strlen(word);
    for (int i = 0; i < length; i++)
    {
        lowercase[i] = tolower(word[i]);
    }
    lowercase[length] = '\0';

    // Compute hash value
    unsigned int index = hash(lowercase);

    // Traverse the linked list at that index in the hash table
    node *cursor = table[index];
    while (cursor != NULL)
    {
        // Compare lowercase word with the current dictionary word
        if (strcmp(cursor->word, lowercase) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number between 0 and N-1, inclusive
unsigned int hash(const char *word)
{
    // A simple hash function: sum the ASCII values of characters
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash += tolower(word[i]);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Read words from file and create nodes for each word
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for the word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            return false;
        }

        // Copy the word into the node
        strcpy(new_node->word, word);

        // Compute hash value
        unsigned int index = hash(word);

        // Insert node into hash table
        new_node->next = table[index];
        table[index] = new_node;
    }

    // Close dictionary file
    fclose(file);

    return true;
}

// Returns the number of words in dictionary, or 0 if not yet loaded
unsigned int size(void)
{
    int word_count = 0;
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            word_count++;
            cursor = cursor->next;
        }
    }
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
