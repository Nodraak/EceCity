#ifndef EC_GRAPHIC_H
#define EC_GRAPHIC_H

/*
    global struct + func
*/
double ec_graphic_scale_x_coord_to_pxl(s_vector c);
double ec_graphic_scale_y_coord_to_pxl(s_vector c);
double ec_graphic_scale_x_pxl_to_coord(s_vector c);
double ec_graphic_scale_y_pxl_to_coord(s_vector c);

void ec_graphic_line(BITMAP *s, int x1, int y1, int x2, int y2, int c);
void ec_graphic_rectfill(BITMAP *s, int x1, int y1, int x2, int y2, int c);
void ec_graphic_stretch_sprite(BITMAP *dest, BITMAP *src, int x1, int y1, int size_y);

int ec_graphic_is_in_board(int x, int y);

#endif
