
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "skiing.h"

#define PATH_GT(len1, drop1, len2, drop2) ((len1) == (len2) ? (drop1) > (drop2) : (len1) > (len2))
#define POINT_PATH_GT(p1, p2) PATH_GT((p1)->path_len, (p1)->path_drop, (p2)->path_len, (p2)->path_drop)

struct Map * map_create(int width, int height)
{
    struct Map *map;
    int size;

    map = malloc(sizeof(struct Map));
    memset(map, 0, sizeof(struct Map));
    map->width = width;
    map->height = height;
    size = MAP_SIZE(map);
    map->map = malloc(sizeof(struct Point)*size);
    memset(map->map, 0, sizeof(struct Point)*size);

    return map;
}

void map_destroy(struct Map *map)
{
    free(map->map);
    free(map);
}

struct Map * map_create_stream(FILE *stream, int width, int height)
{
    struct Map *map;
    struct Point *point;
    int n, x, y;

    map = map_create(width, height);
    errno = 0;
    for( y = 0 ; y < map->height ; y++ ) {
        for( x = 0 ; x < map->width ; x++ ) {
            point = MAP_POINT(map, x, y);
            n = fscanf(stream, "%u", &point->height);
            if( n != 1 ) {
                map_destroy(map);
                return NULL;
            }

            point->x = x;
            point->y = y;
        }
    }

    return map;
}

struct Map * map_create_array(int *array, int width, int height)
{
    struct Map *map;
    struct Point *point;
    int x, y;

    map = map_create(width, height);
    for( y = 0 ; y < map->height ; y++ ) {
        for( x = 0 ; x < map->width ; x++ ) {
            point = MAP_POINT(map, x, y);
            point->height = array[x+y*width];
            point->x = x;
            point->y = y;
        }
    }

    return map;
}

int map_find_path(struct Map *map, int x, int y)
{
    struct Point *point, *points[4];
    int i, len, drop;

    point = MAP_POINT(map, x, y);
    if( !point )
        return -1;

    if( point->path_found )
        return point->path_len;

    point->path_len = 1;
    point->path_drop = 0;
    point->next = NULL;

    points[0] = MAP_UP(map, x, y);
    points[1] = MAP_LEFT(map, x, y);
    points[2] = MAP_DOWN(map, x, y);
    points[3] = MAP_RIGHT(map, x, y);
    for( i = 0 ; i < 4 ; i++ ) {
        if( points[i] && points[i]->height < point->height ) {
            if( !points[i]->path_found )
                map_find_path(map, points[i]->x, points[i]->y);

            len = points[i]->path_len + 1;
            drop = points[i]->path_drop + point->height - points[i]->height;
            if( PATH_GT(len, drop, point->path_len, point->path_drop) ) {
                point->next = points[i];
                point->path_len = len;
                point->path_drop = drop;
            }
        }
    }

    point->path_found = 1;

    return point->path_len;
}

struct Point * map_find_longest_path(struct Map *map)
{
    struct Point *point, *best = NULL;
    int i, mapsize;

    mapsize = MAP_SIZE(map);
    for( i = 0 ; i < mapsize ; i++ ) {
        point = MAP_POINT_INDEX(map, i);
        if( !point->path_found ) {
            map_find_path(map, point->x, point->y);
            if( !best || POINT_PATH_GT(point, best) )
                best = point;
        }
    }

    return best;
}
