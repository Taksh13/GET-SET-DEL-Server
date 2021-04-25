#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

/* Initialize table. This function does't free items memory ! */
void hash_table_init(hash_table *table, uint64_t size) {
    table->items = (hash_table_item *) calloc(size, sizeof(hash_table_item));
    table->size = size;
    table->used_items_count = 0;
    table->not_deleted_used_items_count = 0;
}

/* Insert key value to the table */
uint64_t hash_table_insert(hash_table *table, char *key, char *value) {
    uint64_t curr_index;
    hash_table_item *curr_item;
    for (uint64_t iteration=0; iteration <= table->size; ++iteration) {
        curr_index = hash_string_iterable(&table->hash_params, table->size, iteration, key);
        curr_item = &table->items[curr_index];
        /* Insert on empty or deleted item */
        if (curr_item->is_deleted || !curr_item->is_init) {
            if (!curr_item->is_deleted) {
                table->used_items_count++;
            }
            curr_item->is_deleted = 0;
            curr_item->is_init = 1;
            curr_item->key = key;
            curr_item->value = value;
            table->not_deleted_used_items_count++;
            return iteration;
        }
        /* Rewrite existed item */
        if (strcmp(curr_item->key, key) == 0) {
            curr_item->key = key;
            return iteration;
        }
    }
    return -1;
}

/* Search for key in hash table */
hash_table_item *hash_table_search(hash_table *table, char *key) {
    uint64_t curr_index;
    hash_table_item *curr_item;
    for (uint64_t iteration=0; iteration <= table->size; ++iteration) {
        curr_index = hash_string_iterable(&table->hash_params, table->size, iteration, key);
        curr_item = &table->items[curr_index];
        // Found
        if (curr_item->is_init && !curr_item->is_deleted && (strcmp(curr_item->key, key) == 0)) {
            return curr_item;
        }
        // Search completed
        if (!curr_item->is_init) {
            return NULL;
        }
    }
    return NULL;
}

/* Delete item by key and free key, value memory */
uint8_t hash_table_delete_item_by_key(hash_table *table, char *key) {
    hash_table_item *found_item;
    found_item = hash_table_search(table, key);
    if (!found_item) {
        return 0;
    }
    table->not_deleted_used_items_count--;
    found_item->is_deleted = 1;
    free(found_item->key);
    free(found_item->value);
    return 1;
}

/* Destroy table, clear memory */
void hash_table_destroy(hash_table *table, uint8_t clear_keys) {
    hash_table_item *curr_item;
    if (clear_keys) {
        for (uint64_t i = 0; i < table->size; i++) {
            curr_item = &table->items[i];
            if (curr_item->is_init) {
                free(curr_item->key);
                free(curr_item->value);
            }
        }
    }
    free(table->items);
    table->size = 0;
    table->used_items_count = 0;
    table->not_deleted_used_items_count = 0;
}

/* Increase table size */
uint8_t hash_table_extend(hash_table *table, uint64_t new_size) {
    if (table->size >= new_size) {
        return -1;
    }
    // Initialize temporarty table
    hash_table temp_table;
    hash_table_init(&temp_table, new_size);
    // Insert items to the new table
    hash_table_item *curr_item;
    for (uint64_t i=0; i < table->size; i++) {
        curr_item = &table->items[i];
        if (curr_item->is_init && !curr_item->is_deleted) {
            hash_table_insert(&temp_table, curr_item->key, curr_item->value);
        }
    }
    // Free memory of old table
    hash_table_destroy(table, 0);
    // Set new hash params
    table->hash_params = temp_table.hash_params;
    table->items = temp_table.items;
    table->size = new_size;
    table->used_items_count = temp_table.used_items_count;
    table->not_deleted_used_items_count = temp_table.used_items_count;
    return 1;
}
