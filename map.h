/*
 * Copyright Ted Meyer 2015
 *
 * see LICENSE for details
 *
 */

#ifndef MAP_H_
#define MAP_H_

#define DEFAULT_SIZE 16

#define map_each(m, k, v) \
    for(kvpl*X=(m)->body;X;X=X->next) \
        if(k=X->key,v=X->val,1)

typedef
struct kvpl {
    char *key;
    void *val;
    struct kvpl *next;
} kvpl;

typedef
struct map {
    struct kvpl *body; // null terminated list of KVPs
    unsigned int size;
} dmap;

void *map_put(struct map *map, char *key, void *val);
struct map *map_init(unsigned int size);
void *map_get(struct map *map, char *key);
struct map *map_new();

#endif
