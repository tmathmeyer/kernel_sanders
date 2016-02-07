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
    for(unsigned int i=0;i<(m)->size;i++) \
        for(kvpl*itr=((m)->body)[i];itr;itr=itr->next) \
            for((k)=itr->kvp->key,(v)=itr->kvp->val;(k);(k)=0,v=0)

struct kvp {
    char *key;
    void *val;
};

typedef
struct kvpl {
    struct kvp *kvp;
    struct kvpl *next;
} kvpl;

typedef
struct map {
    struct kvpl **body; // null terminated list of KVPs
    unsigned int size;
} dmap;

void *map_put(struct map *map, char *key, void *val);
struct map *map_init(unsigned int size);
void *map_get(struct map *map, char *key);
struct map *map_new();

#endif
