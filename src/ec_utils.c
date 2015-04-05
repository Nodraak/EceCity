
#include <stdlib.h>

#include "ec_utils.h"
#include "ec_allegro.h"

s_vector *ec_utils_vector_alloc(double x, double y)
{
    s_vector *ret = malloc(sizeof(s_vector));
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
