
/*

    todo

    for each building, implement
        build : add current building on the board
        destroy
        on build : update current building when other buildings are built
        on destroy
        on click / selected ?
        on clock_tick

    water
    elec
    house

    -> to handle water / elec supply for house when the map is changed

*/

#include <stdio.h>

#include "ec_allegro.h"
#include "ec_building.h"

#define BIG_NUM  (1000*1000*1000)

s_building buildings_data[BUILDING_LAST] = {
/*  building                sprite  price       people      elec_created    elec_linked     water_created   water_linked */
    {BUILDING_NONE,         NULL,   -1,         -1,         -1,             -1,             -1,             -1},
    {BUILDING_INFRA_ROAD,   NULL,   10,         0,          0,              0,              0,              0},
    {BUILDING_INFRA_ELEC,   NULL,   10,         0,          0,              0,              0,              0},
    {BUILDING_INFRA_WATER,  NULL,   10,         0,          0,              0,              0,              0},
    {BUILDING_HOUSE_NONE,   NULL,   1000,       0,          0,              0,              0,              0},
    {BUILDING_HOUSE_SMALL,  NULL,   BIG_NUM,    10,         0,              0,              0,              0},
    {BUILDING_HOUSE_MEDIUM, NULL,   BIG_NUM,    50,         0,              0,              0,              0},
    {BUILDING_HOUSE_LARGE,  NULL,   BIG_NUM,    100,        0,              0,              0,              0},
    {BUILDING_HOUSE_XL,     NULL,   BIG_NUM,    1000,       0,              0,              0,              0},
    {BUILDING_SUPPLY_ELEC,  NULL,   100000,     0,          1000,           0,              0,              0},
    {BUILDING_SUPPLY_WATER, NULL,   100000,     0,          0,              0,              1000,           0}
};

/*
    supply : caserne
    special : usines, les centrales, les casernes, Education + distractions (écoles, collèges, lycées, universités,
        écoles d’ingénieurs, bibliothèques, des parcs, des stades, ...)
*/

void ec_building_init(void)
{
    int i;
    char tmp[1024];

    char *img_files[BUILDING_LAST] = {
        "none.bmp",
        "infra_road.bmp",
        "infra_elec.bmp",
        "infra_water.bmp",
        "house_none.bmp",
        "house_small.bmp",
        "house_medium.bmp",
        "house_large.bmp",
        "house_xl.bmp",
        "supply_elec.bmp",
        "supply_water.bmp"
    };

    for (i = 0; i < BUILDING_LAST; ++i)
    {
        sprintf(tmp, "res/%s", img_files[i]);

        buildings_data[i].sprite = load_bmp(tmp, NULL);

        if (buildings_data[i].sprite == NULL)
        {
            sprintf(tmp, "load_bitmap() - res/%s - %d", img_files[i], i);
            printf("%s\n", allegro_error);
            ec_abort(tmp);
        }
    }
}

void ec_building_free(void)
{
    int i;

    for (i = 0; i < BUILDING_LAST; ++i)
        destroy_bitmap(buildings_data[i].sprite);
}

