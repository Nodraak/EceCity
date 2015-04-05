
#include <stdlib.h>

#include "ec_utils.h"


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

