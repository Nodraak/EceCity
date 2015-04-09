
#include <stdlib.h>
#include <stdio.h>

#include "ec_utils.h"
#include "ec_allegro.h"

s_vector2d *ec_utils_vector2d_alloc(double x, double y)
{
    s_vector2d *ret = ec_utils_malloc(sizeof(s_vector2d));
    ret->x = x;
    ret->y = y;
    return ret;
}

s_vector2d ec_utils_vector2d_make(double x, double y)
{
    s_vector2d ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

s_vector2i *ec_utils_vector2i_alloc(int x, int y)
{
    s_vector2i *ret = ec_utils_malloc(sizeof(s_vector2i));
    ret->x = x;
    ret->y = y;
    return ret;
}

s_vector2i ec_utils_vector2i_make(int x, int y)
{
    s_vector2i ret;
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
        char msg[1024];
        sprintf(msg, "malloc %d %s.\n", line, file);
        ec_utils_abort(msg);
    }

    return ret;
}

void _ec_utils_abort(char *error, char *file, int line)
{
    char msg[1024];

    sprintf(msg, "Error !\nLine %d in %s\n%s", line, &file[4], error);
    allegro_message(msg);
    allegro_exit();
    exit(EXIT_FAILURE);
}
