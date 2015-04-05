#ifndef EC_UTILS_H
#define EC_UTILS_H

typedef struct  _s_vector /* TODO : vector2i vector2f */
{
    double      x;
    double      y;
}               s_vector;

s_vector *ec_utils_vector_alloc(double x, double y);
s_vector ec_utils_vector_make(double x, double y);

int ec_utils_pxl_is_in_board(int x, int y);
int ec_utils_cell_is_in_board(int x, int y);

#endif
