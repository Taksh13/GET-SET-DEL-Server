#ifndef _TABLE_H
#define _TABLE_H

/**
 * Hash table interface
 */

#include "hash.h"

typedef struct tableItem tableItem;
struct tableItem
{
    char *key;
    char *value;
    uint8_t is_deleted;
    uint8_t is_init;
};

typedef struct hash_table hash_table;
struct hash_table
{
    uint64_t size;
    uint64_t used_items_count;
    uint64_t not_deleted_used_items_count;
    hash_function_params hash_params;
    tableItem *items;
};

void startHashTable(hash_table *table, uint64_t size);
uint64_t insertHashTable(hash_table *table, char *key, char *value);
tableItem *searchHashTable(hash_table *table, char *key);
uint8_t deleteItemKey(hash_table *table, char *key);
void deleteHashTable(hash_table *table, uint8_t clear_keys);
uint8_t extendHashTable(hash_table *table, uint64_t new_size);

#endif
