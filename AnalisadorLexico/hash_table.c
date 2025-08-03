#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state_machine.h" // Note: adjust if you mean hash_table.h instead!
#include "hash_table.h"

// List of reserved keywords
char *reservedWords[] = {
    "CALL", "BEGIN", "END", "IF", "THEN", "WHILE", "DO", "ODD", "CONST", "VAR", "PROCEDURE"};

// Hash function to compute the index
unsigned long compute_hash(char *str)
{
    unsigned long hash = 5381;
    for (; *str != '\0'; str++)
    {
        hash = ((hash << 5) + hash) + *str; // hash * 33 + c
    }
    return hash % HASH_TABLE_CAPACITY;
}

// Initialize the hash table
HashEntry **create_hash_table()
{
    HashEntry **table = malloc(HASH_TABLE_CAPACITY * sizeof(HashEntry *));
    if (table == NULL)
    {
        return NULL;
    }

    memset(table, 0, HASH_TABLE_CAPACITY * sizeof(HashEntry *));

    size_t reservedCount = sizeof(reservedWords) / sizeof(reservedWords[0]);
    for (size_t i = 0; i < reservedCount; ++i)
    {
        add_entry(table, reservedWords[i]);
    }

    return table;
}

// Insert a word into the hash table
void add_entry(HashEntry **table, char *word)
{
    unsigned long idx = compute_hash(word);
    HashEntry *entry = malloc(sizeof(HashEntry));
    if (entry == NULL)
        return;

    entry->text = strdup(word);
    entry->next = table[idx];
    table[idx] = entry;
}

// Search for a word in the hash table
int find_entry(HashEntry **table, char *word)
{
    unsigned long idx = compute_hash(word);
    for (HashEntry *node = table[idx]; node != NULL; node = node->next)
    {
        if (strcmp(node->text, word) == 0)
            return 1;
    }
    return 0;
}

// Free all memory used by the hash table
void destroy_hash_table(HashEntry **table)
{
    for (int i = 0; i < HASH_TABLE_CAPACITY; ++i)
    {
        HashEntry *node = table[i];
        while (node)
        {
            HashEntry *next = node->next;
            free(node->text);
            free(node);
            node = next;
        }
    }
    free(table);
}
