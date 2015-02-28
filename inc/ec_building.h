#ifndef EC_BUILDING_H
#define EC_BUILDING_H

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

typedef struct      _s_building
{
    e_building      building;
    int             price;
    int             people;
    int             elec_created;
    int             elec_linked;
    int             water_created;
    int             water_linked;
}                   s_building;

extern s_building buildings_data[BUILDING_LAST];

#endif
