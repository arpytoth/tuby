#include <stdlib.h>

#include "map.h"

Map *map_new(int (*compare)(const void *, const void *),
             void (*release_key)(void *key),
             void (*release_value)(void *value))
{
    Map *map;
    MapNode *temp;

    map = (Map*)malloc(sizeof(Map));
    map->compare = compare;
    map->release_key = release_key;
    map->release_value = release_value;

    temp = map->nil = (MapNode*)malloc(sizeof(MapNode));
    temp->parent = tempt->left = temp->right = temp;
    temp->is_red = 0;
    temp->key = NULL;
    temp = map->root = (MapNode*)malloc(sizeof(MapNode));
    temp->parent = temp->left = temp->right = map->nil;
    temp->key = NULL;
    temp->is_red = 0;
    return map;
}

MapNode *map_insert(Map *map, void *key, void *value);

MapNode *map_delete(Map *map, Node *node);

void map_destory(Map *map);

MapNode *map_tree_predecessor(Map *map, MapNode *node);

MapNode *map_tree_successor(Map *map, MapNode *node);

MapNode *find(Map *map, void *key);

