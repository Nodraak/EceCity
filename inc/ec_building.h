#ifndef EC_BUILDING_H
#define EC_BUILDING_H

#include <allegro.h>

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
    int             required;       /* does this building needs it for working */
    int             used;           /* units of ressource used : for houses */
    int             produced;       /* units of ressource produced : for building_supply_* */
}                   s_ressource;

typedef struct      _s_building
{
    e_building      building;
    BITMAP          *sprite;

    s_vector        pos;
    s_vector        size;
    int             price;
    int             people;

    s_ressource     elec;
    s_ressource     water;
    int             is_working;     /* does the building has enough ressources to work */
}                   s_building;

extern s_building building_data[BUILDING_LAST];

void ec_building_init(void);
void ec_building_free(void);
void ec_building_render(s_building *cur, int coord_x, int coord_y);
s_building *ec_building_new(s_building *template, int y, int x);
int ec_building_have_space(int board_y, int board_x, s_vector size);

#endif
