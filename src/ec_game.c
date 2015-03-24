

#include "stdlib.h"
#include <string.h>

#include "ec_game.h"
#include "ec_graphic.h"
#include "ec_building.h"

s_game game;

void ec_timer_time_callback(void)
{
    game.time += 1;
}

void ec_game_init(void)
{
    memset(&game, 0, sizeof(s_game));
    game.money = 500000;

    install_int(ec_timer_time_callback, 1000);
}

char *building_enum_to_str[BUILDING_LAST] = {
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

void ec_game_on_button_left(void)
{
    int pxl_x, pxl_y, board_x, board_y;

    pxl_x = ec_graphic_scale_x_pxl_to_coord(window.mousePos.x);
    pxl_y = ec_graphic_scale_y_pxl_to_coord(window.mousePos.y);
    board_x = pxl_x/BOARD_SIZE;
    board_y = pxl_y/BOARD_SIZE;

    if (ec_graphic_is_in_board(pxl_x, pxl_y)
        && ec_building_have_space(board_y, board_x, building_data[game.building_selected].size)
        && game.building_selected != BUILDING_NONE
        && game.money >= building_data[game.building_selected].price)
    {
        int x, y;
        s_building *new = ec_building_new(&building_data[game.building_selected], board_y, board_x);

        for (y = 0; y < new->size.y; ++y)
        {
            for (x = 0; x < new->size.x; ++x)
            {
                game.board[board_y+y][board_x+x] = new;
            }
        }
        /* TODO : update elec + water distribution */
    }

    window.mouseButtonLeft = 0;
}


void ec_board_render(BITMAP *s)
{
    int i, j;

    /* board */
    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            if (game.board[j][i] != NULL && game.board[j][i]->pos.x == i && game.board[j][i]->pos.y == j)
                ec_building_render(game.board[j][i], i*BOARD_SIZE, j*BOARD_SIZE);
        }
    }

    /* hover */
    if (game.building_selected != BUILDING_NONE)
    {
        int coord_x = ec_graphic_scale_x_pxl_to_coord(window.mousePos.x);
        int coord_y = ec_graphic_scale_y_pxl_to_coord(window.mousePos.y);
        int size_x = building_data[game.building_selected].size.x * BOARD_SIZE;
        int size_y = building_data[game.building_selected].size.y * BOARD_SIZE;

        if (ec_graphic_is_in_board(coord_x, coord_y))
        {
            coord_x = coord_x/BOARD_SIZE*BOARD_SIZE;
            coord_y = coord_y/BOARD_SIZE*BOARD_SIZE;

            ec_graphic_rectfill(s, coord_x, coord_y, coord_x+size_x, coord_y+size_y, makecol(64, 64, 64));
            ec_graphic_stretch_sprite(
                s, building_data[game.building_selected].sprite,
                coord_x, coord_y,
                coord_x+size_x, coord_y+size_y
            );
        }
    }

    /* lines */
    for (i = 0; i < BOARD_LINE+1; ++i)
        ec_graphic_line(s, 0, i*BOARD_SIZE, BOARD_WIDTH, i*BOARD_SIZE, makecol(128, 128, 128));
    for (i = 0; i < BOARD_COL+1; ++i)
        ec_graphic_line(s, i*BOARD_SIZE, 0, i*BOARD_SIZE, BOARD_HEIGHT, makecol(128, 128, 128));
}

void ec_game_render(BITMAP *s)
{
    int i;

    /* menu */
    rectfill(s, 0, 0, 150, WINDOW_HEIGHT, makecol(200, 200, 200));

    textprintf_ex(s, font, 10, 20, makecol(0, 0, 0), -1, "timer : %d - %d", game.time, game.time/30);
    textprintf_ex(s, font, 10, 40, makecol(0, 0, 0), -1, "%d EceFlouz", game.money);

    textprintf_ex(s, font, 10, 80, makecol(0, 0, 0), -1, "people : %d", game.people);
    textprintf_ex(s, font, 10, 100, makecol(0, 0, 0), -1, "elec : %d/%d", game.elec_used, game.elec_capacity);
    textprintf_ex(s, font, 10, 120, makecol(0, 0, 0), -1, "water : %d/%d", game.water_used, game.water_capacity);

    for (i = 0; i < BUILDING_LAST; ++i)
        textprintf_ex(s, font, 30, 160+20*i, makecol(0, 0, 0), -1, "%s", building_enum_to_str[i]);
    textprintf_ex(s, font, 10, 160+20*game.building_selected, makecol(0, 0, 0), -1, "->");
}

