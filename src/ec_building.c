
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

void ec_building_render(BITMAP *s, s_building *cur, int coord_x, int coord_y, int x, int y)
{
    if (game.layer == 0)
    {
        /* sprite */
        ec_graphic_stretch_sprite(s, cur, coord_x, coord_y);

        /* if not connected to water or elec, show sign */
        if (!cur->is_working)
        {
            double i;

            for (i = 0; i < 3; i+=0.1)
            {
                int coord_x2 = coord_x + cur->size.x*BOARD_SIZE;
                int coord_y2 = coord_y + cur->size.y*BOARD_SIZE;

                ec_graphic_line(s, coord_x, coord_y+i, coord_x2, coord_y+i, makecol(128, 0, 0));
                ec_graphic_line(s, coord_x2-i, coord_y, coord_x2-i, coord_y2, makecol(128, 0, 0));
            }
        }
    }
    else
    {
        int w = game.board[y][x]->size.x;
        int h = game.board[y][x]->size.y;
        s_vector2d v1 = ec_utils_vector2d_make(x*BOARD_SIZE,                y*BOARD_SIZE);
        s_vector2d v2 = ec_utils_vector2d_make(x*BOARD_SIZE,                y*BOARD_SIZE+h*BOARD_SIZE);
        s_vector2d v3 = ec_utils_vector2d_make(x*BOARD_SIZE+w*BOARD_SIZE,   y*BOARD_SIZE+h*BOARD_SIZE);
        s_vector2d v4 = ec_utils_vector2d_make(x*BOARD_SIZE+w*BOARD_SIZE,   y*BOARD_SIZE);

        if (game.board[y][x]->type == BUILDING_INFRA_ROAD)
        {
            int c = 0;

            if (game.layer == 1)
                c = makecol(50, 50, 230);
            else if (game.layer == 2)
                c = makecol(230, 230, 50);

            ec_graphic_polygon(s, v1, v2, v3, v4, c);
        }
        else
        {
            int actual = -1, max = -1;

            /* gray building */
            s_vector2i pos = window.scale_coord_to_pxl(v1);
            ec_graphic_polygon(s, v1, v2, v3, v4, makecol(190, 190, 190));

            /* get resrc stat */
            if (ec_building_is_house(game.board[y][x]->type))
            {
                s_ressource*(*get_resrc)(s_building *b) = NULL;

                if (game.layer == 1)
                    get_resrc = ec_building_resrc_get_water;
                else if (game.layer == 2)
                    get_resrc = ec_building_resrc_get_elec;

                actual = get_resrc(game.board[y][x])->used;
                max = get_resrc(&building_data[game.board[y][x]->type])->used;
            }
            else if (game.layer == 2 && game.board[y][x]->type == BUILDING_SUPPLY_ELEC)
            {
                actual = ec_building_resrc_get_elec(game.board[y][x])->produced;
                max = ec_building_resrc_get_elec(&building_data[game.board[y][x]->type])->produced;
            }
            else if (game.layer == 1 && game.board[y][x]->type == BUILDING_SUPPLY_WATER)
            {
                actual = ec_building_resrc_get_water(game.board[y][x])->produced;
                max = ec_building_resrc_get_water(&building_data[game.board[y][x]->type])->produced;
            }

            /* show colored stat */
            if (actual != -1 && max != -1)
            {
                int c = 0;
                if (actual == max)
                    c = makecol(0, 128, 00);
                else if (actual == 0)
                    c = makecol(230, 50, 50);
                else
                    c = makecol(240, 120, 0);

                textprintf_ex(s, font, pos.x+20, pos.y-4, c, -1, "%d/%d", actual, max);
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

    if (!ec_building_is_house(template->type))
        ret->is_working = 1;

    game.people += template->people;
    game.elec_capacity += template->elec.produced;
    game.water_capacity += template->water.produced;

    return ret;
}

void ec_building_new(int board_y, int board_x)
{
    int i, j;
    s_building *new = NULL;

    new = ec_building_alloc(&building_data[game.building_selected], board_y, board_x);
    new->evolved = game.time;

    game.money -= building_data[game.building_selected].price;

    for (j = 0; j < new->size.y; ++j)
    {
        for (i = 0; i < new->size.x; ++i)
            game.board[board_y+j][board_x+i] = new;

    }

    ec_game_on_building_new();
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
