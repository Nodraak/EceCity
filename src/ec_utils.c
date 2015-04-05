
#include <stdlib.h>
#include <stdio.h>

#include "ec_utils.h"
#include "ec_allegro.h"

s_vector *ec_utils_vector_alloc(double x, double y)
{
    s_vector *ret = ec_utils_malloc(sizeof(s_vector));
    ret->x = x;
    ret->y = y;
    return ret;
}


s_vector ec_utils_vector_make(double x, double y)
{
    s_vector ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

int ec_utils_pxl_is_in_board(int x, int y)
{
    return (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT);
}

int ec_utils_cell_is_in_board(int x, int y)
{
    return (x >= 0 && x < BOARD_COL && y >= 0 && y < BOARD_LINE);
}

void *_ec_utils_malloc(size_t size, char *file, int line)
{
    void *ret = malloc(size);

    if (ret == NULL)
    {
        printf("Error malloc %d %s.\n", line, file);
    }

    return ret;
}
