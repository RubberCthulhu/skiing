
#ifndef SKIING_H
#define SKIING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

struct Point {
    int x;
    int y;
    int height;
    int path_found;
    int path_len;
    int path_drop;
    struct Point *next;
};

struct Map {
    struct Point *map;
    int width;
    int height;
};

#define MAP_SIZE(map)            ((map)->width * (map)->height)
#define POINT_VALID(point, max)  ((point) >= 0 && (point) < (max))
#define MAP_POINT(map, x, y)     (POINT_VALID(x, (map)->width) && POINT_VALID(y, (map)->height) \
                                    ? &(map)->map[(x)+(y)*(map)->width] \
                                    : NULL)
#define MAP_POINT_INDEX(map, i)  (POINT_VALID(i, MAP_SIZE(map)) ? &(map)->map[i] : NULL)
#define MAP_UP(map, x, y)        MAP_POINT(map, x, (y)-1)
#define MAP_DOWN(map, x, y)      MAP_POINT(map, x, (y)+1)
#define MAP_LEFT(map, x, y)      MAP_POINT(map, (x)-1, y)
#define MAP_RIGHT(map, x, y)     MAP_POINT(map, (x)+1, y)

struct Map * map_create(int width, int height);
void map_destroy(struct Map *map);
struct Map * map_create_stream(FILE *stream, int width, int height);
struct Map * map_create_array(int *array, int width, int height);
int map_find_path(struct Map *map, int x, int y);
struct Point * map_find_longest_path(struct Map *map);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SKIING_H */
