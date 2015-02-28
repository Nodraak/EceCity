#ifndef EC_BUILDING_H
#define EC_BUILDING_H

#include <allegro.h>

typedef enum        _e_building
{
    BUILDING_NONE,

    BUILDING_INFRA_ROAD,
    BUILDING_INFRA_ELEC,

    BUILDING_INFRA_WATER,
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
    int             is_connected;   /* is connected to the network */
}                   s_ressource;

typedef struct      _s_building
{
    e_building      building;
    BITMAP          *sprite;

    int             price;
    int             people;

    s_ressource     elec;
    s_ressource     water;
}                   s_building;

extern s_building building_data[BUILDING_LAST];
void ec_building_init(void);
void ec_building_free(void);
void ec_building_render(s_building *building, int coord_x, int coord_y);
void ec_building_new(s_building *dest, s_building *template);

#endif
