
#include <allegro.h>

#include "ec_allegro.h"
#include "ec_allegro_graphic.h"

void _scale_and_call(void(*f)(BITMAP*, int, int, int, int, int), BITMAP* s, int x1, int y1, int x2, int y2, int c)
{
    x1 = ec_allegro_graphic_scale_x_coord_to_pxl(x1);
    y1 = ec_allegro_graphic_scale_y_coord_to_pxl(y1);
    x2 = ec_allegro_graphic_scale_x_coord_to_pxl(x2);
    y2 = ec_allegro_graphic_scale_y_coord_to_pxl(y2);

    f(s, x1, y1, x2, y2, c);
}

void ec_allegro_graphic_line(BITMAP *s, int x1, int y1, int x2, int y2, int c)
{
    _scale_and_call(line, s, x1, y1, x2, y2, c);
}

void ec_allegro_graphic_rectfill(BITMAP *s, int x1, int y1, int x2, int y2, int c)
{
    _scale_and_call(rectfill, s, x1, y1, x2, y2, c);
}

void ec_allegro_graphic_stretch_sprite(BITMAP *dest, BITMAP *src, int x1, int y1, int x2, int y2)
{
    int x1_scaled = ec_allegro_graphic_scale_x_coord_to_pxl(x1);
    int y1_scaled = ec_allegro_graphic_scale_y_coord_to_pxl(y1);
    int x2_scaled = ec_allegro_graphic_scale_x_coord_to_pxl(x2) - x1_scaled;
    int y2_scaled = ec_allegro_graphic_scale_y_coord_to_pxl(y2) - y1_scaled;

    stretch_sprite(dest, src, x1_scaled, y1_scaled, x2_scaled, y2_scaled);
}

int ec_allegro_graphic_is_in_board(int x, int y)
{
    return (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT);
}
