#ifndef EC_GRAPHIC_H
#define EC_GRAPHIC_H

#include "ec_building.h"
#include "ec_utils.h"

/*
    global struct + func
*/
s_vector2i ec_graphic_scale_coord_to_pxl(s_vector2d c);
double ec_graphic_scale_x_pxl_to_coord(s_vector2i c);
double ec_graphic_scale_y_pxl_to_coord(s_vector2i c);

void ec_graphic_line(BITMAP *s, double x1, double y1, double x2, double y2, int c);
void ec_graphic_rectfill(BITMAP *s, int x1, int y1, int x2, int y2, int c);
void ec_graphic_stretch_sprite(BITMAP *dest, s_building *b, int x1, int y1);
void ec_graphic_putpixel(BITMAP *s, double x, double y, int c);

#endif
