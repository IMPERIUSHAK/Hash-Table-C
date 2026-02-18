#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

/* 
initialisation functions for ht_item. This function allocates a chunk
of memory the size of an ht_item, and saves a copy of the strings k and v
int the new chunk of memory. The function is marked as static because it will only be called by code internal
to the hash table.
*/

//init
static ht_item* ht_new_item(const char* k, const char* v){

    ht_item* i = malloc(sizeof(ht_item));
    
    i->key = strdup(k);
    i->value = strdup(v);
    
    return i;

}
/*
ht_new initialises a new hash table. `size` defines how many items we can store. 
This is fixed at 53 for now. I'll expand it later.
*/
ht_hash_table* ht_new(){

    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;

}

/*
We also need functions for deleting `ht_items` and `ht_hash_tables`
which `free` the memory we've allocated, so we dont' cause `memory leaks`.
*/
static void ht_del_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht){
    
    for(int i = 0; i < ht->size; i++){
        
        ht_item* item = ht->items[i];

        if(item != NULL){
            ht_del_item(item);
        }

    }

    free(ht->items);
    free(ht);
}