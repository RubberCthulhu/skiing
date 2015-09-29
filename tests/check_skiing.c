
#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "skiing.h"

struct Map *map;
int basic_array[] = {1, 2, 3, 4};
int basic_array_width = 2, basic_array_height = 2;

void init_map_basic(void)
{
    map = map_create_array(basic_array, basic_array_width, basic_array_height);
}

void end_map_basic(void)
{
    map_destroy(map);
}

START_TEST(test_map_size)
{
    ck_assert_int_eq(MAP_SIZE(map), basic_array_width*basic_array_height);
}
END_TEST

START_TEST(test_map_points)
{
    ck_assert_int_eq((MAP_POINT(map, 0, 0))->height, 1);
    ck_assert_int_eq((MAP_POINT(map, 1, 0))->height, 2);
    ck_assert_int_eq((MAP_POINT(map, 0, 1))->height, 3);
    ck_assert_int_eq((MAP_POINT(map, 1, 1))->height, 4);
}
END_TEST

START_TEST(test_map_directions)
{
    struct Point *point = MAP_POINT(map, 1, 1);

    struct Point *up = MAP_UP(map, point->x, point->y);
    ck_assert_int_eq(up->height, 2);
    ck_assert_int_eq(up->x, 1);
    ck_assert_int_eq(up->y, 0);

    struct Point *down = MAP_DOWN(map, point->x, point->y);
    ck_assert_ptr_eq(down, NULL);

    struct Point *left = MAP_LEFT(map, point->x, point->y);
    ck_assert_int_eq(left->height, 3);
    ck_assert_int_eq(left->x, 0);
    ck_assert_int_eq(left->y, 1);

    struct Point *right = MAP_DOWN(map, point->x, point->y);
    ck_assert_ptr_eq(right, NULL);
}
END_TEST

START_TEST(test_map_find_path)
{
    struct Point *point;

    map_find_path(map, 0, 0);
    map_find_path(map, 1, 0);
    map_find_path(map, 0, 1);
    map_find_path(map, 1, 1);

    // 1
    point = MAP_POINT(map, 0, 0);
    ck_assert_int_eq(point->path_len, 1);
    ck_assert_int_eq(point->path_drop, 0);

    // 2
    point = MAP_POINT(map, 1, 0);
    ck_assert_int_eq(point->path_len, 2);
    ck_assert_int_eq(point->path_drop, 1);

    // 3
    point = MAP_POINT(map, 0, 1);
    ck_assert_int_eq(point->path_len, 2);
    ck_assert_int_eq(point->path_drop, 2);

    // 4
    point = MAP_POINT(map, 1, 1);
    ck_assert_int_eq(point->path_len, 3);
    ck_assert_int_eq(point->path_drop, 3);
}
END_TEST

START_TEST(test_map_find_longest_path)
{
    struct Point *point = map_find_longest_path(map);
    ck_assert_int_eq(point->height, 4);
    ck_assert_int_eq(point->x, 1);
    ck_assert_int_eq(point->y, 1);
    ck_assert_int_eq(point->path_len, 3);
    ck_assert_int_eq(point->path_drop, 3);
}
END_TEST

START_TEST(test_longest_path_example)
{
    int array[] = {
        4, 8, 7, 3,
        2, 5, 9, 3,
        6, 3, 2, 5,
        4, 4, 1, 6
    };
    int width = 4, height = 4;
    struct Map *map = map_create_array(array, width, height);

    struct Point *point = map_find_longest_path(map);
    ck_assert_int_eq(point->height, 9);
    ck_assert_int_eq(point->x, 2);
    ck_assert_int_eq(point->y, 1);
    ck_assert_int_eq(point->path_len, 5);
    ck_assert_int_eq(point->path_drop, 8);

    map_destroy(map);
}
END_TEST

Suite * skiing_suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("Skiing");

    tc = tcase_create("Map Basic");
    tcase_add_checked_fixture(tc, init_map_basic, end_map_basic);
    tcase_add_test(tc, test_map_size);
    tcase_add_test(tc, test_map_points);
    tcase_add_test(tc, test_map_directions);
    tcase_add_test(tc, test_map_find_path);
    tcase_add_test(tc, test_map_find_longest_path);
    suite_add_tcase(s, tc);

    tc = tcase_create("Example Path");
    tcase_add_test(tc, test_longest_path_example);
    suite_add_tcase(s, tc);

    return s;
}

int main(int argc, char **argv)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = skiing_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
