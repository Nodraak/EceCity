#ifndef EC_ALLEGRO_H
#define EC_ALLEGRO_H

/* define */
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600
#define WINDOW_FPS      50

/* struct */
typedef struct  _s_vector
{
    int         x;
    int         y;
}               s_vector;

typedef struct  _s_allegro
{
    BITMAP      *screen;
    int         quit;
    s_vector    mousePos;
    s_vector    mouseRel;
    int         mouseButtonLeft;
    int         key[KEY_MAX];
}               s_allegro;

/* global struct + func */
extern s_allegro window;
void ec_abort(char *error);
void ec_allegro_init(void);
void ec_allegro_update_event(void);
void ec_allegro_free(void);

/* hidden hacks */
void _ec_abort(char *error, char *file, int line);
#define ec_abort(error) _ec_abort(error, __FILE__, __LINE__)

#endif
