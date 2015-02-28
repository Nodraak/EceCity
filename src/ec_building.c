
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

#include "ec_building.h"

#define BIG_NUM  (1000*1000*1000)

s_building buildings_data[BUILDING_LAST] = {
    {BUILDING_NONE,        -1,     -1,     -1,     -1,     -1,     -1},
    {BUILDING_INFRA_ROAD,   10,     0,      0,      0,      0,      0},
    {BUILDING_INFRA_ELEC,   10,     0,      0,      0,      0,      0},
    {BUILDING_INFRA_WATER,  10,     0,      0,      0,      0,      0},
    {BUILDING_HOUSE_NONE,   1000,   0,      0,      0,      0,      0},
    {BUILDING_HOUSE_SMALL,  BIG_NUM, 10,    0,      0,      0,      0},
    {BUILDING_HOUSE_MEDIUM, BIG_NUM, 50,    0,      0,      0,      0},
    {BUILDING_HOUSE_LARGE,  BIG_NUM, 100,   0,      0,      0,      0},
    {BUILDING_HOUSE_XL,     BIG_NUM, 1000,  0,      0,      0,      0},
    {BUILDING_SUPPLY_ELEC,  100000, 0,      1000,   0,      0,      0},
    {BUILDING_SUPPLY_WATER, 100000, 0,      0,      0,      1000,   0}
};

/*
    supply : caserne
    special : usines, les centrales, les casernes, Education + distractions (écoles, collèges, lycées, universités,
        écoles d’ingénieurs, bibliothèques, des parcs, des stades, ...)
*/
