#ifndef EC_BUILDING_H
#define EC_BUILDING_H

#include <allegro.h>

#include "ec_utils.h"

#define BUILDING_EVOLVE_DELAY 5

typedef enum        _e_building
{
    BUILDING_NONE = 0,

    BUILDING_INFRA_ROAD,

    BUILDING_HOUSE_NONE,
    BUILDING_HOUSE_SMALL,
    BUILDING_HOUSE_MEDIUM,
    BUILDING_HOUSE_LARGE,
    BUILDING_HOUSE_XL,

    BUILDING_SUPPLY_ELEC,
    BUILDING_SUPPLY_WATER,

    BUILDING_LAST
}                   e_building;

typedef struct      _s_ressource
{
    int             visited;        /* internal use, cf ec_graph.c */
    int             used;           /* units of ressource used : for houses */
    int             produced;       /* units of ressource produced : for building_supply_* */
}                   s_ressource;

typedef struct      _s_building
{
    e_building      type;
    BITMAP          *sprite_straight;
    BITMAP          *sprite_iso;
    s_vector2i      blit_offset_straight;
    s_vector2i      blit_offset_iso;

    s_vector2i      pos;
    s_vector2i      size;
    int             price;
    int             people;
    int             evolved;        /* time of last evolve */

    s_ressource     elec;
    s_ressource     water;
    int             is_working;     /* does the building has enough ressources to work */
}                   s_building;

extern s_building building_data[BUILDING_LAST];

void ec_building_init_all(void);
void ec_building_free_all(void);
BITMAP *ec_building_get_sprite_straight(s_building *b);
BITMAP *ec_building_get_sprite_iso(s_building *b);
s_vector2i ec_building_get_blit_offset_straight(s_building *b);
s_vector2i ec_building_get_blit_offset_iso(s_building *b);

void ec_building_render(BITMAP *s, s_building *cur, int coord_x, int coord_y, int x, int y);
s_building *ec_building_alloc(s_building *template, int y, int x);
void ec_building_new(int board_y, int board_x);
char *ec_game_building_enum_to_str(e_building type);

int ec_building_have_space(int board_y, int board_x, s_vector2i size);
int ec_building_is_house(e_building type);
s_ressource *ec_building_resrc_get_water(s_building *b);
s_ressource *ec_building_resrc_get_elec(s_building *b);

#endif
