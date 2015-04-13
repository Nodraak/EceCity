
/*

    todo

    -> on clock_tick -> for houses
*/

#include <stdio.h>

#include "ec_allegro.h"
#include "ec_graphic.h"
#include "ec_game.h"
#include "ec_building.h"
#include "ec_utils.h"

#define BIG_NUM  (1000*1000*1000)

s_building building_data[BUILDING_LAST];

/*
    supply : caserne
    special : usines, les centrales, les casernes, Education + distractions (écoles, collèges, lycées, universités,
        écoles d’ingénieurs, bibliothèques, des parcs, des stades, ...)
*/

void ec_building_init_all(void)
{
    int i;
    char tmp[1024];
    s_building *cur = NULL;
    FILE *f = NULL;

    f = fopen("res/building_data.txt", "r");
    if (f == NULL)
        ec_utils_abort("fopen() building_data.txt");

    /* skip help info */
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);
    fgets(tmp, 1024-1, f);

    for (i = 0; i < BUILDING_LAST; ++i)
    {
        cur = &building_data[i];

        memset(cur, 0, sizeof(s_building));
        cur->type = i;

        fgets(tmp, 1024-1, f);

        fgets(tmp, 1024-1, f);
        tmp[strlen(tmp)-1] = '\0';
        cur->sprite_straight = ec_utils_load_sprite(tmp);
        fgets(tmp, 1024-1, f);
        tmp[strlen(tmp)-1] = '\0';
        cur->sprite_iso = ec_utils_load_sprite(tmp);

        fgets(tmp, 1024-1, f);
        sscanf(tmp, "%d %d", &cur->price, &cur->people);
        fgets(tmp, 1024-1, f);
        sscanf(tmp, "%d %d", &cur->elec.used, &cur->elec.produced);
        fgets(tmp, 1024-1, f);
        sscanf(tmp, "%d %d", &cur->water.used, &cur->water.produced);
        fgets(tmp, 1024-1, f);
        sscanf(tmp, "%d %d", &cur->size.x, &cur->size.y);
        fgets(tmp, 1024-1, f);
        sscanf(tmp, "%d %d %d %d", &cur->blit_offset_straight.x, &cur->blit_offset_straight.y,
            &cur->blit_offset_iso.x, &cur->blit_offset_iso.y);
        fgets(tmp, 1024-1, f);
        sscanf(tmp, "%lf %lf", &cur->zoom_straight, &cur->zoom_iso);

        fgets(tmp, 1024-1, f);
    }

    fclose(f);
}

void ec_building_free_all(void)
{
    int i;

    for (i = 0; i < BUILDING_LAST; ++i)
    {
        destroy_bitmap(building_data[i].sprite_straight);
        destroy_bitmap(building_data[i].sprite_iso);
    }
}

BITMAP *ec_building_get_sprite_straight(s_building *b)
{
    return b->sprite_straight;
}

BITMAP *ec_building_get_sprite_iso(s_building *b)
{
    return b->sprite_iso;
}

s_vector2i ec_building_get_blit_offset_straight(s_building *b)
{
    return b->blit_offset_straight;
}

s_vector2i ec_building_get_blit_offset_iso(s_building *b)
{
    return b->blit_offset_iso;
}

s_vector2i ec_building_get_stat_blit_offset_straight(s_building *b)
{
    (void)b;
    return ec_utils_vector2i_make(0, -20);
}

s_vector2i ec_building_get_stat_blit_offset_iso(s_building *b)
{
    (void)b;
    return ec_utils_vector2i_make(20, -4);
}

double ec_building_get_blit_zoom_straight(s_building *b)
{
    return b->zoom_straight;
}

double ec_building_get_blit_zoom_iso(s_building *b)
{
    return b->zoom_iso;
}

void ec_get_resrc(s_building *cur, int *actual, int *max)
{
    if (ec_building_is_house(cur->type))
    {
        s_ressource*(*get_resrc)(s_building *b) = NULL;

        if (game.layer == 1)
            get_resrc = ec_building_resrc_get_water;
        else if (game.layer == 2)
            get_resrc = ec_building_resrc_get_elec;

        *actual = get_resrc(cur)->used;
        *max = get_resrc(&building_data[cur->type])->used;
    }
    else if (game.layer == 2 && cur->type == BUILDING_SUPPLY_ELEC)
    {
        *max = ec_building_resrc_get_elec(&building_data[cur->type])->produced;
        *actual = *max - ec_building_resrc_get_elec(cur)->produced;
    }
    else if (game.layer == 1 && cur->type == BUILDING_SUPPLY_WATER)
    {
        *max = ec_building_resrc_get_water(&building_data[cur->type])->produced;
        *actual = *max - ec_building_resrc_get_water(cur)->produced;
    }

}

int ec_get_color(int actual, int max, e_building type)
{
    int c = 0;

    if (ec_building_is_house(type))
    {
        if (actual == max)
            c = makecol(0, 128, 00);
        else if (actual == 0)
            c = makecol(230, 50, 50);
        else
            c = makecol(240, 120, 0);
    }
    else
    {
        if (actual == 0)
            c = makecol(0, 128, 00);
        else if (actual == max)
            c = makecol(230, 50, 50);
        else
            c = makecol(240, 120, 0);
    }

    return c;
}

void ec_building_render(BITMAP *s, s_building *cur, int x, int y)
{
    s_vector2i coord = ec_utils_vector2i_make(x*BOARD_SIZE, y*BOARD_SIZE);

    int c = 0;
    int w = cur->size.x*BOARD_SIZE;
    int h = cur->size.y*BOARD_SIZE;
    s_vector2d v1 = ec_utils_vector2d_make(coord.x,     coord.y);
    s_vector2d v2 = ec_utils_vector2d_make(coord.x,     coord.y+h);
    s_vector2d v3 = ec_utils_vector2d_make(coord.x+w,   coord.y+h);
    s_vector2d v4 = ec_utils_vector2d_make(coord.x+w,   coord.y);

    if (game.layer == 0)
    {
        if (cur->is_burning)
            c = makecol(230, 50, 50);
        else
            c = makecol(150, 150, 150);
        ec_graphic_polygon(s, v1, v2, v3, v4, c);

        ec_graphic_stretch_sprite(s, cur, coord.x, coord.y);
    }
    else
    {
        /* building polygon */
        if (cur->type != BUILDING_INFRA_ROAD)
        {
            c = makecol(190, 190, 190);
        }
        else
        {
            if (game.layer == 1)
                c = makecol(50, 50, 230);
            else if (game.layer == 2)
                c = makecol(230, 230, 50);
        }

        ec_graphic_polygon(s, v1, v2, v3, v4, c);

        /* building stat */
        if (cur->type != BUILDING_INFRA_ROAD)
        {
            /* get resrc stat */
            int actual = -1, max = -1;
            ec_get_resrc(cur, &actual, &max);

            /* show colored stat */
            if (actual != -1 && max != -1)
            {
                s_vector2i pos = window.scale_coord_to_pxl(ec_utils_vector2d_make(coord.x, coord.y));
                s_vector2i offset = window.building_get_stat_blit_offset(cur);
                int c = ec_get_color(actual, max, cur->type);

                textprintf_ex(s, font, pos.x+offset.x, pos.y+offset.y, c, -1, "%d/%d", actual, max);
            }
        }
    }
}

s_building *ec_building_alloc(s_building *template, int y, int x)
{
    s_building *ret = ec_utils_malloc(sizeof(s_building));

    memcpy(ret, template, sizeof(s_building));
    ret->pos.x = x;
    ret->pos.y = y;

    game.elec_capacity += template->elec.produced;
    game.water_capacity += template->water.produced;

    return ret;
}

void ec_building_new(int board_y, int board_x, e_building type)
{
    int i, j;
    s_building *new = NULL;

    new = ec_building_alloc(&building_data[type], board_y, board_x);
    new->evolved = game.time;

    game.money -= building_data[type].price;

    for (j = 0; j < new->size.y; ++j)
    {
        for (i = 0; i < new->size.x; ++i)
            game.board[board_y+j][board_x+i] = new;
    }
}

int ec_building_evolve(s_building *old, int dir)
{
    int new_type;

    if ((dir == -1 && old->type == BUILDING_HOUSE_NONE) || (dir == 1 && old->type == BUILDING_HOUSE_XL))
    {
        old->evolved = game.time;
        return 0;
    }

    new_type = old->type + dir;
    ec_building_new(old->pos.y, old->pos.x, new_type);

    game.water_capacity -= old->water.produced;
    game.elec_capacity -= old->elec.produced;
    game.money += building_data[new_type].price;

    free(old);
    return 1;
}

char *ec_game_building_enum_to_str(e_building type)
{
    char *data[BUILDING_LAST] = {
        "NONE",
        "INFRA_ROAD",
        "HOUSE_NONE",
        "HOUSE_SMALL",
        "HOUSE_MEDIUM",
        "HOUSE_LARGE",
        "HOUSE_XL",
        "SUPPLY_ELEC",
        "SUPPLY_WATER"
    };

    return data[type];
}

int ec_building_have_space(int board_y, int board_x, s_vector2i size)
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


int ec_building_is_house(e_building type)
{
    return ((type == BUILDING_HOUSE_NONE) || (type == BUILDING_HOUSE_SMALL)
        || (type == BUILDING_HOUSE_MEDIUM) || (type == BUILDING_HOUSE_LARGE)
        || (type == BUILDING_HOUSE_XL));
}

s_ressource *ec_building_resrc_get_water(s_building *b)
{
    return &b->water;
}

s_ressource *ec_building_resrc_get_elec(s_building *b)
{
    return &b->elec;
}
