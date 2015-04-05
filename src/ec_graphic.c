
#include <allegro.h>

#include "ec_allegro.h"
#include "ec_graphic.h"

#include <math.h>

#define M_PI 3.1415926536
#define ANGLE 30

double ec_graphic_scale_x_coord_to_pxl(s_vector c)
{
    return (cos(ANGLE*M_PI/180)*(c.x+c.y) + window.offset.x) * window.zoom;
}

double ec_graphic_scale_y_coord_to_pxl(s_vector c)
{
    return WINDOW_HEIGHT-((sin(ANGLE*M_PI/180)*(c.y-c.x) - window.offset.y) * window.zoom);
}

double ec_graphic_scale_x_pxl_to_coord(s_vector p)
{
    p.y = WINDOW_HEIGHT-p.y;

    p.x = p.x/window.zoom - window.offset.x;
    p.y = p.y/window.zoom + window.offset.y;

    return p.x/(2*cos(ANGLE*M_PI/180)) - p.y/(2*sin(ANGLE*M_PI/180));
}

double ec_graphic_scale_y_pxl_to_coord(s_vector p)
{
    p.y = WINDOW_HEIGHT-p.y;

    p.x = p.x/window.zoom - window.offset.x;
    p.y = p.y/window.zoom + window.offset.y;

    return p.x/(2*cos(ANGLE*M_PI/180)) + p.y/(2*sin(ANGLE*M_PI/180));
}

void _scale_and_call(void(*f)(BITMAP*, int, int, int, int, int), BITMAP* s, s_vector c1, s_vector c2, int c)
{
    int x1_scaled = ec_graphic_scale_x_coord_to_pxl(c1);
    int y1_scaled = ec_graphic_scale_y_coord_to_pxl(c1);
    int x2_scaled = ec_graphic_scale_x_coord_to_pxl(c2);
    int y2_scaled = ec_graphic_scale_y_coord_to_pxl(c2);

    f(s, x1_scaled, y1_scaled, x2_scaled, y2_scaled, c);
}

void ec_graphic_line(BITMAP *s, double x1, double y1, double x2, double y2, int c)
{
    _scale_and_call(line, s, ec_utils_vector_make(x1, y1), ec_utils_vector_make(x2, y2), c);
}

void ec_graphic_rectfill(BITMAP *s, int x1, int y1, int x2, int y2, int c)
{
    _scale_and_call(rectfill, s, ec_utils_vector_make(x1, y1), ec_utils_vector_make(x2, y2), c);
}

void ec_graphic_stretch_sprite(BITMAP *dest, BITMAP *src, int x1, int y1)
{
    fixed angle = ftofix(ANGLE*M_PI/180);
    fixed scale = ftofix(window.zoom/10);
    double vfix = -(src->h/20/10)*(BOARD_SIZE/2*window.zoom);

    int x1_scaled = ec_graphic_scale_x_coord_to_pxl(ec_utils_vector_make(x1, y1));
    int y1_scaled = ec_graphic_scale_y_coord_to_pxl(ec_utils_vector_make(x1, y1));

    rotate_scaled_sprite(dest, src, x1_scaled, y1_scaled+vfix, angle, scale);
}

int ec_graphic_is_in_board(int x, int y)
{
    return (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT);
}
