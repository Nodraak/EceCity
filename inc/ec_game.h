#ifndef EC_GAME_H
#define EC_GAME_H

#define nb_Icon_toolbar 11

#include "ec_allegro.h"
#include "ec_building.h"


typedef struct  _s_game
{
    int         time;
    int         money;

    int         people;
    int         elec_capacity;
    int         elec_used;
    int         water_capacity;
    int         water_used;

    e_building  building_selected;

    s_building  *board[BOARD_LINE][BOARD_COL];
}               s_game;

typedef struct _s_toolbar
{
    BITMAP      *sprite;
    s_vector    pos;

}               s_toolbar;

extern s_game game;

extern s_toolbar toolbar[nb_Icon_toolbar];

void ec_game_init(void);
void ec_game_on_button_left(void);
void ec_game_on_building_new();
void ec_game_render_board(BITMAP *s);
void ec_game_render_menu(BITMAP *s);

BITMAP *_ec_game_load_sprite(char *file);
void ec_game_load_toolbar(void);
void ec_game_free_toolbar(void);

#endif
