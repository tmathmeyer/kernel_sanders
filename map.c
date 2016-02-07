#include "map.h"
#include "alloc.h"
#include "goodstring.h"
#include "sandersio.h"
#include "screentext.h"

struct map *map_init(unsigned int size) {
    struct map *result = mm_zalloc(sizeof(struct map));
    result->size = 0;
    result->body = NULL;
    return result;
}

void *map_put(struct map *map, char *key, void *val) {
    sanders_printf("adding %s\n", key);
    struct kvpl *newl = mm_alloc(sizeof(struct kvpl));

    newl->key = gs_dup(key);
    newl->val = val;
    newl->next = map->body;
    map->body = newl;
    return NULL;
}

void *map_get(struct map *map, char *key) {
    struct kvpl *c_temp = map->body;
    console_print("looking up: ");
    console_print(key);
    console_print("\n");
    while(c_temp) {
        if (gs_comp(key, c_temp->key)==0) {
            return c_temp->val;
        }
        c_temp = c_temp->next;
    }
    return NULL;
}

struct map *map_new() {
    return map_init(DEFAULT_SIZE);
}

