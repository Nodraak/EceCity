#ifndef EC_UTILS_H
#define EC_UTILS_H

typedef struct  _s_vector2d
{
    double      x;
    double      y;
}               s_vector2d;

typedef struct  _s_vector2i
{
    int         x;
    int         y;
}               s_vector2i;

s_vector2d *ec_utils_vector2d_alloc(double x, double y);
s_vector2d ec_utils_vector2d_make(double x, double y);
s_vector2i *ec_utils_vector2i_alloc(int x, int y);
s_vector2i ec_utils_vector2i_make(int x, int y);

int ec_utils_pxl_is_in_board(int x, int y);
int ec_utils_cell_is_in_board(int x, int y);

void *_ec_utils_malloc(size_t size, char *file, int line);
void *ec_utils_malloc(size_t size);
void _ec_utils_abort(char *error, char *file, int line);
void ec_utils_abort(char *error);

#define ec_utils_malloc(size) _ec_utils_malloc(size, __FILE__, __LINE__)
#define ec_utils_abort(error) _ec_utils_abort(error, __FILE__, __LINE__)

#endif
