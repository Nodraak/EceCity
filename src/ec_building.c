
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
#include "ec_allegro_graphic.h"
#include "ec_game.h"
#include "ec_building.h"

#define BIG_NUM  (1000*1000*1000)

s_building building_data[BUILDING_LAST];

/*
    supply : caserne
    special : usines, les centrales, les casernes, Education + distractions (écoles, collèges, lycées, universités,
        écoles d’ingénieurs, bibliothèques, des parcs, des stades, ...)
*/

BITMAP *ec_building_load_sprite(char *file)
{
    BITMAP *ret = NULL;
    char tmp[1024];

    sprintf(tmp, "res/%s", file);

    ret = load_bmp(tmp, NULL);
    if (ret == NULL)
    {
        sprintf(tmp, "load_bitmap() - %s", tmp);
        ec_abort(tmp);
    }

    return ret;
}

void ec_building_init(void)
{
    int i;
    char tmp[1024];
    s_building *cur = NULL;
    FILE *f = NULL;

    f = fopen("res/building_data.txt", "r");
    if (f == NULL)
        ec_abort("fopen() building_data.txt");

    /* skip help info */
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);

    for (i = 0; i < BUILDING_LAST; ++i)
    {
        cur = &building_data[i];

        fgets(tmp, 1024-1, f);

        fgets(tmp, 1024-1, f);
        tmp[strlen(tmp)-1] = '\0';
        cur->sprite = ec_building_load_sprite(tmp);

        fscanf(f, "%d %d", &cur->price, &cur->people);
        fscanf(f, "%d %d %d", &cur->elec.required, &cur->elec.used, &cur->elec.produced);
        fscanf(f, "%d %d %d", &cur->water.required, &cur->water.used, &cur->water.produced);

        cur->building = i;
        cur->elec.is_connected = 0;
        cur->water.is_connected = 0;

        fgets(tmp, 1024-1, f);
        fgets(tmp, 1024-1, f);
    }

    fclose(f);
}

void ec_building_free(void)
{
    int i;

    for (i = 0; i < BUILDING_LAST; ++i)
        destroy_bitmap(building_data[i].sprite);
}

void ec_building_render(s_building *building, int coord_x, int coord_y)
{
    int sprite_id = building->building;

    /* sprite */
    ec_allegro_graphic_stretch_sprite(
        window.screen, building_data[sprite_id].sprite,
        coord_x, coord_y, coord_x+BOARD_SIZE, coord_y+BOARD_SIZE
    );

    /* if not connected to water or elec, show sign */
    if ((building->elec.required && !building->elec.is_connected)
        || (building->water.required && !building->water.is_connected)
    )
    {
        ec_allegro_graphic_rectfill(window.screen, coord_x+BOARD_SIZE-10, coord_y, coord_x+BOARD_SIZE, coord_y+10, makecol(128, 0, 0));
    }
}

void ec_building_new(s_building *dest, s_building *template)
{
    memcpy(dest, template, sizeof(s_building));

    game.money -= template->price;
    game.people += template->people;

    /* house */
    if (template->building == BUILDING_HOUSE_NONE) /* TODO : or house_medium or ... */
    {
        game.water_used += template->water.used;
    }

    /* supply */
    if (template->building == BUILDING_SUPPLY_ELEC)
    {
        dest->elec.is_connected = 1;
        game.elec_capacity += template->elec.produced;
    }
    if (template->building == BUILDING_SUPPLY_WATER)
    {
        dest->elec.is_connected = 1;
        game.water_capacity += template->water.produced;
    }
}
