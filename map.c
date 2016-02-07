#include "map.h"
#include "alloc.h"
#include "goodstring.h"
#include "sandersio.h"
#include "screentext.h"

struct map *map_init(unsigned int size) {
    struct map *result = mm_zalloc(sizeof(struct map));
    result->body = mm_zalloc(sizeof(struct kvp*) * size);
    result->size = size;
    for(int i=0;i<size;i++) {
        (result->body)[i] = 0;
    }
    return result;
}

unsigned int hash(char *key) {
    unsigned int result = 0;
    while(*key) {
        result += *key;
        key++;
    }
    return result;
}

void *map_put(struct map *map, char *key, void *val) {
    unsigned int h = hash(key);
    struct kvpl *c_list = (map->body)[h % map->size];
    struct kvpl *c_temp = c_list;
    while(c_temp) {
        if (gs_comp(key, c_temp->kvp->key)==0) {
            void *old = c_temp->kvp->val;
            c_temp->kvp->val = val;
            return old;
        }
        c_temp = c_temp->next;
    }

    struct kvp *new = mm_zalloc(sizeof(struct kvp));
    new->key = key;
    new->val = val;
    
    
    struct kvpl *newl = mm_zalloc(sizeof(struct kvpl));
    newl->kvp = new;
    newl->next = c_list;
    (map->body)[h % map->size] = c_list = newl;

    return NULL;
}

void *map_get(struct map *map, char *key) {
    unsigned int h = hash(key);
    struct kvpl *c_temp = (map->body)[h % map->size];
    while(c_temp) {
        if (gs_comp(key, c_temp->kvp->key)==0) {
            return c_temp->kvp->val;
        }
        c_temp = c_temp->next;
    }
    return NULL;
}

struct map *map_new() {
    return map_init(DEFAULT_SIZE);
}

