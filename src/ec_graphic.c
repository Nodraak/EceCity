
#include <allegro.h>

#include "ec_allegro.h"
#include "ec_graphic.h"

#include <math.h>

#ifndef M_PI
    #define M_PI 3.1415926536
#endif
#define ANGLE 30

/* iso */

s_vector2i ec_graphic_scale_coord_to_pxl_iso(s_vector2d c)
{
    s_vector2i ret;
    ret.x = (cos(ANGLE*M_PI/180)*(c.x+c.y) + window.offset.x) * window.zoom;
    ret.y = (sin(ANGLE*M_PI/180)*(c.y-c.x) - window.offset.y) * window.zoom;
    ret.y = WINDOW_HEIGHT-ret.y;
    return ret;
}

s_vector2d ec_graphic_scale_pxl_to_coord_iso(s_vector2i p)
{
    s_vector2d ret, tmp;

    p.y = WINDOW_HEIGHT-p.y;

    tmp.x = p.x/window.zoom - window.offset.x;
    tmp.y = p.y/window.zoom + window.offset.y;
    ret.x = tmp.x/(2*cos(ANGLE*M_PI/180)) - tmp.y/(2*sin(ANGLE*M_PI/180));

    tmp.x = p.x/window.zoom - window.offset.x;
    tmp.y = p.y/window.zoom + window.offset.y;
    ret.y = tmp.x/(2*cos(ANGLE*M_PI/180)) + tmp.y/(2*sin(ANGLE*M_PI/180));

    return ret;
}

/* straight */

s_vector2i ec_graphic_scale_coord_to_pxl_straight(s_vector2d c)
{
    s_vector2i ret;
    ret.x = (c.x + window.offset.x) * window.zoom;
    ret.y = (c.y - window.offset.y) * window.zoom;
    ret.y = WINDOW_HEIGHT-ret.y;
    return ret;
}

s_vector2d ec_graphic_scale_pxl_to_coord_straight(s_vector2i p)
{
    s_vector2d ret;
    p.y = WINDOW_HEIGHT-p.y;
    ret.x = (p.x / window.zoom) - window.offset.x;
    ret.y = (p.y / window.zoom) + window.offset.y;
    return ret;
}

/* graphics */

void _scale_and_call(void(*f)(BITMAP*, int, int, int, int, int), BITMAP* s, s_vector2d c1, s_vector2d c2, int c)
{
    s_vector2i v1 = window.scale_coord_to_pxl(c1);
    s_vector2i v2 = window.scale_coord_to_pxl(c2);

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
    BITMAP *src = window.building_get_sprite(b);
    fixed scale = ftofix(window.zoom*window.building_get_blit_zoom(b));
    double fix_x = -window.building_get_blit_offset(b).x * window.zoom;
    double fix_y = -window.building_get_blit_offset(b).y * window.zoom;
    s_vector2i pos = window.scale_coord_to_pxl(ec_utils_vector2d_make(x1, y1));

    rotate_scaled_sprite(dest, src, pos.x+fix_x, pos.y+fix_y, ftofix(0), scale);
}

void ec_graphic_putpixel(BITMAP *s, double x, double y, int c)
{
    s_vector2i scaled = window.scale_coord_to_pxl(ec_utils_vector2d_make(x, y));

    putpixel(s, scaled.x, scaled.y, c);
}

void ec_graphic_polygon(BITMAP *s, s_vector2d v1, s_vector2d v2, s_vector2d v3, s_vector2d v4, int c)
{
    int vertices[4*2];

    s_vector2i v1_s = window.scale_coord_to_pxl(v1);
    s_vector2i v2_s = window.scale_coord_to_pxl(v2);
    s_vector2i v3_s = window.scale_coord_to_pxl(v3);
    s_vector2i v4_s = window.scale_coord_to_pxl(v4);

    vertices[0] = v1_s.x;
    vertices[1] = v1_s.y;
    vertices[2] = v2_s.x;
    vertices[3] = v2_s.y;
    vertices[4] = v3_s.x;
    vertices[5] = v3_s.y;
    vertices[6] = v4_s.x;
    vertices[7] = v4_s.y;

    polygon(s, 4, vertices, c);
}
