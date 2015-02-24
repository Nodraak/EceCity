
#include <allegro.h>

#include <ec_allegro.h>
#include <ec_allegro_graphic.h>

void ec_allegro_line(BITMAP *screen, int x1, int y1, int x2, int y2, int c)
{
    x1 = ec_allegro_scale_x_coord_to_pxl(x1);
    y1 = ec_allegro_scale_y_coord_to_pxl(y1);
    x2 = ec_allegro_scale_x_coord_to_pxl(x2);
    y2 = ec_allegro_scale_y_coord_to_pxl(y2);

    line(screen, x1, y1, x2, y2, c);
}
