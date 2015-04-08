
#include <allegro.h>

#include "ec_allegro.h"
#include "ec_graphic.h"

#include <math.h>

#ifndef M_PI
    #define M_PI 3.1415926536
#endif
#define ANGLE 30

s_vector2i ec_graphic_scale_coord_to_pxl(s_vector2d c)
{
    s_vector2i ret;
    ret.x = (cos(ANGLE*M_PI/180)*(c.x+c.y) + window.offset.x) * window.zoom;
    ret.y = WINDOW_HEIGHT-((sin(ANGLE*M_PI/180)*(c.y-c.x) - window.offset.y) * window.zoom);
    return ret;
}

double ec_graphic_scale_x_pxl_to_coord(s_vector2i p)
{
    p.y = WINDOW_HEIGHT-p.y;

    p.x = p.x/window.zoom - window.offset.x;
    p.y = p.y/window.zoom + window.offset.y;

    return p.x/(2*cos(ANGLE*M_PI/180)) - p.y/(2*sin(ANGLE*M_PI/180));
}

double ec_graphic_scale_y_pxl_to_coord(s_vector2i p)
{
    p.y = WINDOW_HEIGHT-p.y;

    p.x = p.x/window.zoom - window.offset.x;
    p.y = p.y/window.zoom + window.offset.y;

    return p.x/(2*cos(ANGLE*M_PI/180)) + p.y/(2*sin(ANGLE*M_PI/180));
}

void _scale_and_call(void(*f)(BITMAP*, int, int, int, int, int), BITMAP* s, s_vector2d c1, s_vector2d c2, int c)
{
    s_vector2i v1 = ec_graphic_scale_coord_to_pxl(c1);
    s_vector2i v2 = ec_graphic_scale_coord_to_pxl(c2);

    f(s, v1.x, v1.y, v2.x, v2.y, c);
}


void ec_graphic_line(BITMAP *s, double x1, double y1, double x2, double y2, int c)
{
    _scale_and_call(line, s, ec_utils_vector2d_make(x1, y1), ec_utils_vector2d_make(x2, y2), c);
}

void ec_graphic_rectfill(BITMAP *s, int x1, int y1, int x2, int y2, int c)
{
    _scale_and_call(rectfill, s, ec_utils_vector2d_make(x1, y1), ec_utils_vector2d_make(x2, y2), c);
}

void ec_graphic_stretch_sprite(BITMAP *dest, s_building *b, int x1, int y1)
{
    fixed angle = ftofix(ANGLE*M_PI/180);
    fixed scale = ftofix(window.zoom/10);
    double fix_pxl = -b->size.y*(BOARD_SIZE/2*window.zoom);

    s_vector2i scaled = ec_graphic_scale_coord_to_pxl(ec_utils_vector2d_make(x1, y1));

    rotate_scaled_sprite(dest, b->sprite, scaled.x, scaled.y+fix_pxl, angle, scale);
}

void ec_graphic_putpixel(BITMAP *s, double x, double y, int c)
{
    s_vector2i scaled = ec_graphic_scale_coord_to_pxl(ec_utils_vector2d_make(x, y));

    putpixel(s, scaled.x, scaled.y, c);
}
