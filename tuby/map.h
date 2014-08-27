#ifndef _TUBY_MAP_H_
#define _TUBY_MAP_H_

typedef struct MapNode
{
    void *key;
    void *value;
    int is_red;
    struct MapNode *left;
    struct MapNode *right;
    struct MapNode *parent;
} MapNode;

typedef struct Map
{
    int (*compare)(const void *a, const void *b);
    void (*release_key)(void *key);
    void (*release_value)(void *value);
    MapNode *root;
    MapNode *nil;
} Map;

Map *map_new(int (*compare)(const void *, const void *),
             void (*release_key)(void *key),
             void (*release_value)(void *value));

MapNode *map_insert(Map *map, void *key, void *value);

MapNode *map_delete(Map *map, Node *node);

void map_destory(Map *map);

MapNode *map_tree_predecessor(Map *map, MapNode *node);

MapNode *map_tree_successor(Map *map, MapNode *node);

MapNode *find(Map *map, void *key);
#endif // _TUBY_MAP_H_

