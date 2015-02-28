

#include "stdlib.h"

#include "ec_game.h"
#include "ec_allegro_graphic.h"
#include "ec_building.h"

s_game game;

void ec_timer_time_callback(void)
{
    game.time += 1;
}

void ec_game_init(void)
{
    game.time = 0;
    game.money = 500000;
    game.people = 0;
    game.elec_capacity = 0;
    game.elec_used = 0;
    game.water_capacity = 0;
    game.water_used = 0;
    game.building_selected = BUILDING_NONE;

    install_int(ec_timer_time_callback, 1000);
}

char *building_enum_to_str[BUILDING_LAST] = {
    "NONE",
    "INFRA_ROAD",
    "INFRA_ELEC",
    "INFRA_WATER",
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

    pxl_x = ec_allegro_graphic_scale_x_pxl_to_coord(window.mousePos.x);
    pxl_y = ec_allegro_graphic_scale_y_pxl_to_coord(window.mousePos.y);
    board_x = pxl_x/BOARD_SIZE;
    board_y = pxl_y/BOARD_SIZE;

    /* board ok */
    /* building + game ok */
    if (ec_allegro_graphic_is_in_board(pxl_x, pxl_y) && game.board[board_y][board_x].building == BUILDING_NONE
        && game.building_selected != BUILDING_NONE && game.money >= building_data[game.building_selected].price)
    {
        ec_building_new(&game.board[board_y][board_x], &building_data[game.building_selected]);
        /* TODO : update elec + water distribution */
    }

    window.mouseButtonLeft = 0;
}


void ec_board_render(BITMAP *s)
{
    int i, j, coord_x, coord_y;

    /* board */
    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            if (game.board[j][i].building != BUILDING_NONE)
                ec_building_render(&game.board[j][i], i*BOARD_SIZE, j*BOARD_SIZE);
        }
    }

    /* hover */
    coord_x = ec_allegro_graphic_scale_x_pxl_to_coord(window.mousePos.x);
    coord_y = ec_allegro_graphic_scale_y_pxl_to_coord(window.mousePos.y);

    if (ec_allegro_graphic_is_in_board(coord_x, coord_y))
    {
        coord_x = coord_x/BOARD_SIZE*BOARD_SIZE;
        coord_y = coord_y/BOARD_SIZE*BOARD_SIZE;

        ec_allegro_graphic_rectfill(s, coord_x, coord_y, coord_x+BOARD_SIZE, coord_y+BOARD_SIZE, makecol(64, 64, 64));
    }

    for (i = 0; i < BOARD_LINE+1; ++i)
        ec_allegro_graphic_line(s, 0, i*BOARD_SIZE, BOARD_WIDTH, i*BOARD_SIZE, makecol(128, 128, 128));
    for (i = 0; i < BOARD_COL+1; ++i)
        ec_allegro_graphic_line(s, i*BOARD_SIZE, 0, i*BOARD_SIZE, BOARD_HEIGHT, makecol(128, 128, 128));
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

