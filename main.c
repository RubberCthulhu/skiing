
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "skiing.h"

int main(int argc, char *argv[])
{
    FILE *stream;
    struct Map *map;
    struct Point *best = NULL, *point;
    int n, width, height, mapsize;

    if( argc == 1 ) {
        stream = stdin;
    }
    else {
        if( (stream = freopen(argv[1], "r", stdin)) == NULL ) {
            printf("Error: Load map from %s: %s\n", argv[1], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    errno = 0;
    n = fscanf(stream, "%u %u", &width, &height);
    if( n != 2 ) {
        if( errno != 0 )
            printf("Error: Load map from %s: %s\n",
                (argc == 1 ? "STDIN" : argv[1]), strerror(errno));
        else
            printf("Error: Incorrect input format\n");
    }

    if( (map = map_create_stream(stream, width, height)) == NULL ) {
        if( errno != 0 )
            printf("Error: Load map from %s: %s\n",
                (argc == 1 ? "STDIN" : argv[1]), strerror(errno));
        else
            printf("Error: Incorrect input format\n");
        exit(EXIT_FAILURE);
    }

    fclose(stream);

    mapsize = MAP_SIZE(map);
    if( mapsize == 0 ) {
        printf("Error: The map is empty\n");
        exit(EXIT_FAILURE);
    }

    best = map_find_longest_path(map);
    printf("%d %d", best->path_len, best->path_drop);
    for( point = best ; point != NULL ; point = point->next ) {
        printf(" %d(%d %d)", point->height, point->x, point->y);
    }
    printf("\n");

    map_destroy(map);

    exit(EXIT_SUCCESS);
}
