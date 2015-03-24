
/*

    todo

    -> on clock_tick -> for houses
    -> update water / elec supply for house when the map is changed

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

BITMAP *ec_building_load_sprite(char *file) /* TODO : prepend _ for func name ? */
{
    BITMAP *ret = NULL;
    char tmp1[1024], tmp2[1024];

    sprintf(tmp1, "res/%s", file);

    ret = load_bmp(tmp1, NULL);
    if (ret == NULL)
    {
        sprintf(tmp2, "load_bitmap() - %s", tmp1);
        ec_abort(tmp2);
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
        fscanf(f, "%lf %lf", &cur->size.x, &cur->size.y);

        cur->building = i;
        cur->is_working = 0;

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

void ec_building_render(s_building *cur, int coord_x, int coord_y)
{
    /* sprite */
    ec_allegro_graphic_stretch_sprite(
        window.screen, cur->sprite,
        coord_x, coord_y,
        coord_x+BOARD_SIZE*cur->size.x, coord_y+BOARD_SIZE*cur->size.y
    );

    /* if not connected to water or elec, show sign */
    if (cur->is_working)
    {
        ec_allegro_graphic_rectfill(window.screen, coord_x+BOARD_SIZE-10, coord_y, coord_x+BOARD_SIZE, coord_y+10, makecol(128, 0, 0));
    }
}

s_building *ec_building_new(s_building *template, int y, int x)
{
    s_building *ret = malloc(sizeof(s_building)); /* TODO check error */

    memcpy(ret, template, sizeof(s_building));
    ret->pos.x = x;
    ret->pos.y = y;

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
        ret->is_working = 1;
        game.elec_capacity += template->elec.produced;
    }
    if (template->building == BUILDING_SUPPLY_WATER)
    {
        ret->is_working = 1;
        game.water_capacity += template->water.produced;
    }

    return ret;
}

int ec_building_have_space(int board_y, int board_x, s_vector size)
{
    int x, y;

    for (y = 0; y < size.y; ++y)
    {
        for (x = 0; x < size.x; ++x)
        {
            if (game.board[board_y+y][board_x+x] != NULL)
                return 0;
        }
    }

    return 1;
}
