#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

// Capacity of the hash table
#define HASH_TABLE_CAPACITY 50

// Structure for hash table entry
typedef struct HashEntry
{
    char *text;
    struct HashEntry *next;
}HashEntry;

// Hash function prototype
unsigned long compute_hash(char *str);

// Initialize the hash table
HashEntry **create_hash_table();

// Insert a word into the hash table
void add_entry(HashEntry **table, char *word);

// Search for a word in the hash table
int find_entry(HashEntry **table, char *word);

// Free the allocated memory for the hash table
void destroy_hash_table(HashEntry **table);

#endif
