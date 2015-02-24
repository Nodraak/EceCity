#ifndef EC_ALLEGRO_GRAPHIC_H
#define EC_ALLEGRO_GRAPHIC_H

/*
    global struct + func
*/
int ec_allegro_graphic_scale_x_coord_to_pxl(double val);
int ec_allegro_graphic_scale_y_coord_to_pxl(double val);
double ec_allegro_graphic_scale_x_pxl_to_coord(int val);
double ec_allegro_graphic_scale_y_pxl_to_coord(int val);

void ec_allegro_graphic_line(BITMAP *s, int x1, int y1, int x2, int y2, int c);
void ec_allegro_graphic_rectfill(BITMAP *s, int x1, int y1, int x2, int y2, int c);

int ec_allegro_graphic_is_in_board(int x, int y);

/*
    hidden hacks and heavily optimized things
*/

/* These functions are heavily optimized. See the test_optimize/ folder for more info. */
#define _ec_allegro_graphic_scale_coord_to_pxl(window_zoom, window_offset, val) (((val)+(window_offset)) * (window_zoom))
#define ec_allegro_graphic_scale_x_coord_to_pxl(val) _ec_allegro_graphic_scale_coord_to_pxl(window.zoom, window.offset.x, (val))
#define ec_allegro_graphic_scale_y_coord_to_pxl(val) _ec_allegro_graphic_scale_coord_to_pxl(window.zoom, window.offset.y, (val))

#define _ec_allegro_graphic_scale_pxl_to_coord(window_zoom, window_offset, val) ((val) / (window_zoom) - (window_offset))
#define ec_allegro_graphic_scale_x_pxl_to_coord(val) _ec_allegro_graphic_scale_pxl_to_coord(window.zoom, window.offset.x, (val))
#define ec_allegro_graphic_scale_y_pxl_to_coord(val) _ec_allegro_graphic_scale_pxl_to_coord(window.zoom, window.offset.y, (val))

#endif
