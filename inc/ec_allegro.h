#ifndef EC_ALLEGRO_H
#define EC_ALLEGRO_H

#include <allegro.h>
#include "ec_utils.h"

/*
    define
*/
#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   768
#define WINDOW_FPS      50

#define BOARD_MOVE_MOUSE_AREA 50

#define BOARD_COL       45 /* number of col */
#define BOARD_LINE      35 /* number of line */
#define BOARD_SIZE      20 /* size of one square cell */
#define BOARD_WIDTH     (BOARD_COL*BOARD_SIZE)
#define BOARD_HEIGHT    (BOARD_LINE*BOARD_SIZE)

#define BOARD_MOVE_SPEED    10
#define BOARD_ZOOM_FACTOR   0.01

/*
    struct
*/
typedef struct  _s_window
{
    BITMAP      *screen;
    int         quit;

    s_vector2i  mousePos;
    int         mouseButtonLeft;
    int         key[KEY_MAX];

    double      zoom;
    s_vector2i  offset;
}               s_window;

/*
    global struct + func
*/

extern s_window window;

void ec_abort(char *error);
void ec_allegro_init(void);
void ec_allegro_update_event(void);
void ec_allegro_free(void);

#endif
