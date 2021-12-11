// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10007;
unsigned int count = 0;
unsigned int hash_value = 0;
unsigned int nodesize = sizeof(node);
// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
     hash_value = hash(word);
     node *cursor = table[hash_value];
     while (cursor != NULL) {
         if (!strcasecmp(cursor->word, word)) {
             return true;
         }
         cursor = cursor->next;
     }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    
    // sdbm hash function -> lalaland.txt
    // WORDS MISSPELLED:     955
    // WORDS IN DICTIONARY:  143091
    // WORDS IN TEXT:        17756
    // TIME IN load:         0.02
    // TIME IN check:        0.02
    // TIME IN size:         0.00
    // TIME IN unload:       0.01
    // TIME IN TOTAL:        0.05
    unsigned long hash = 0;
    int c;
    while ((c = toupper(*word++))) {
        hash = (hash << 6) + (hash << 16) - hash + c;
    }
    
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) {
        return false;
    }

    char line[LENGTH + 1];

    while (fscanf(file, "%s", line) != EOF) {
        node *n = malloc(nodesize);
        if (n == NULL) {
            return false;
        }
        strcpy(n->word, line);
        hash_value = hash(line);
        n->next = table[hash_value];
        table[hash_value] = n;
        count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++) {
        node *cursor = table[i];
        while (cursor != NULL) {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
