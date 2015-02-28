#ifndef EC_GAME_H
#define EC_GAME_H

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

    s_building  board[BOARD_LINE][BOARD_COL];
}               s_game;

extern s_game game;
void ec_game_init(void);
void ec_game_on_button_left(void);
void ec_board_render(BITMAP *s);
void ec_game_render(BITMAP *s);

#endif
