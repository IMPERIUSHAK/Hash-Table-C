#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#define HT_PRIME_1 151
#define HT_PRIME_2 163

static ht_item HT_DELETED_ITEM = {NULL, NULL};

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


static int ht_hash(const char* s, const int a, const int m){

    long hash = 0;
    const int  len_s = strlen(s);

    for(int i = 0; i < len_s; i++){
        hash += (long)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }

    return (int)hash;
}

static int ht_get_hash(
    const char* s, const int num_buckets, const int attempt
){
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

/*
To insert a new key-value pair, we iterate through indexes until we find
an empty backet. We then insert the item into that bucket and increment
the hash table's count attribute, to indicate a new item has been added
*/

void ht_insert(ht_hash_table *ht, const char* key, const char* value) {
    
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* curr_item = ht->items[index];
    int  i = 1;

    while(curr_item != NULL){
       
        if (curr_item != &HT_DELETED_ITEM) {
            if (strcmp(curr_item->key, key) == 0) {
                ht_del_item(curr_item);
                ht->items[index] = item;
                return;
            }
        }

        index = ht_get_hash(item->key, ht->size, i);
        curr_item = ht->items[index];
        i++;

    }

    ht->items[index] = item;
    ht->count++;
}

/*
Searching is similar to inserting, but at each iteration of the `while` loop,
we check whether the item's key matches the key we're searching for. if it does,
we return the item's value. If the while loop hits a `NULL bucket, we return
`NULL`, to indicate that no value was found.
*/

char* ht_search(ht_hash_table* ht, const char* key){
    
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while(item != NULL){
        
        if(item != &HT_DELETED_ITEM){

            if(strcmp(item->key, key) == 0){
                return item->value;
            }
        }    
        
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
            
    }

    return NULL;
}

/*
Deleting from an open addressed hash table is more complicated than inserting or searching.
The item we wish to delete may be part of collision chain. Removing it from the table will break that chain,
and will make find items in the tail of the chain impossible. To solve this, instead of deleting
the item, we simply mark it as deleted.
*/

void ht_delete(ht_hash_table* ht, const char* key){

    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while(item != NULL){

        if(item != &HT_DELETED_ITEM){
            
            if(strcmp(item->key, key) == 0){
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }

        }
        
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }

    ht->count--;
}