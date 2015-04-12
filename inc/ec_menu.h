#ifndef EC_MENU_H
#define EC_MENU_H

#include <stdlib.h>
#include <stdio.h>

#include "ec_allegro.h"
#include "ec_utils.h"
#include "ec_game.h"

typedef enum        _e_menu_item
{
    MENU_MAIN_PLAY = 0,
    MENU_MAIN_LOAD,
    MENU_MAIN_RULES,
    MENU_MAIN_QUIT,

    MENU_PAUSE_RESUME,
    MENU_PAUSE_LOAD,
    MENU_PAUSE_SAVE,
    MENU_PAUSE_MENU,
    MENU_PAUSE_QUIT,

    MENU_LAST
}                   e_menu;

typedef struct _s_menu_item
{
    BITMAP      *sprite;
    s_vector2i  pos;
}               s_menu_item;

typedef struct _s_menu
{
    char        quit;
    char        stop;

    BITMAP      *background;
    BITMAP      *pause;
    s_menu_item      **item;
}       s_menu;

void ec_menu_handle_event(s_menu *menu);
s_menu *ec_menu_load(void);
int ec_menu_item_get_hovered(s_menu *menu, int start, int nbItem);
void ec_menu_render(s_menu *menu, BITMAP *fond, int start, int nbItem);
void ec_menu_free(s_menu *menu);
void ec_menu_menu(s_menu *menu);

///PAUSE

void ec_menu_pause_event(s_menu *menu);

void ec_menu_handle_pause(s_menu *menu);

#endif
