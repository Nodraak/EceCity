#ifndef EC_GAME_H
#define EC_GAME_H

#define TOOLBAR_WIDTH       150

#include "ec_allegro.h"
#include "ec_building.h"

typedef enum        _e_toolbar
{
    TOOLBAR_INFO_TIME = 0,
    TOOLBAR_INFO_MONEY,
    TOOLBAR_INFO_PEOPLE,
    TOOLBAR_INFO_ELEC,
    TOOLBAR_INFO_WATER,

    TOOLBAR_SAVE,
    TOOLBAR_PAUSE,
    TOOLBAR_PLAY,
    TOOLBAR_LEVEL_0,
    TOOLBAR_LEVEL_MINUS_1,
    TOOLBAR_LEVEL_MINUS_2,

    TOOLBAR_BUILDING_ROAD,
    TOOLBAR_BUILDING_HOUSE,
    TOOLBAR_BUILDING_ELEC,
    TOOLBAR_BUILDING_WATER,

    TOOLBAR_LAST
}                   e_toolbar;

typedef struct  _s_game
{
    int         time;
    int         money;
    int         pause;
    int         quit;

    int         people;
    int         elec_capacity;
    int         elec_used;
    int         water_capacity;
    int         water_used;
    int         layer;

    e_building  building_selected;

    s_building  *board[BOARD_LINE][BOARD_COL];
}               s_game;

typedef struct _s_toolbar
{
    BITMAP      *sprite;
    s_vector2i  pos;
}               s_toolbar;

extern s_game game;

extern s_toolbar toolbar[TOOLBAR_LAST];

void ec_game_init(void);
void ec_game_free(void);

int ec_game_is_on_sprite(int posx, int posy, int taillex, int tailley);
void ec_game_on_button_left(void);
void ec_game_on_building_new();

void ec_game_render_board(BITMAP *s);
void ec_game_render_menu(BITMAP *s);

BITMAP *_ec_game_load_sprite(char *file);
void ec_game_load_toolbar(void);
void ec_game_free_toolbar(void);

void ec_game_evolve(void);

#endif
