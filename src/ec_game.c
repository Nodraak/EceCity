

#include "stdlib.h"

#include "ec_game.h"
#include "ec_allegro_graphic.h"

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

    install_int(ec_timer_time_callback, 1000);
}

void ec_game_on_button_left(void)
{

    int pxl_x, pxl_y, coord_x, coord_y;

    pxl_x = ec_allegro_graphic_scale_x_pxl_to_coord(window.mousePos.x);
    pxl_y = ec_allegro_graphic_scale_y_pxl_to_coord(window.mousePos.y);

    coord_x = pxl_x/BOARD_SIZE;
    coord_y = pxl_y/BOARD_SIZE;

    if (ec_allegro_graphic_is_in_board(pxl_x, pxl_y) && game.board[coord_y][coord_x].building == BUILDING_NONE)
    {
        int selected = BUILDING_INFRA_ROAD;

        memcpy(
            &game.board[coord_y][coord_x],
            &buildings_data[selected],
            sizeof(s_building)
        );

        game.money -= buildings_data[selected].price;
        game.people += buildings_data[selected].people;
    }

    window.mouseButtonLeft = 0;
}


void ec_board_render(BITMAP *s)
{
    int i, j, coord_x, coord_y;

    /* board */
    for (i = 0; i < BOARD_LINE+1; ++i)
        ec_allegro_graphic_line(s, 0, i*BOARD_SIZE, BOARD_WIDTH, i*BOARD_SIZE, makecol(128, 128, 128));
    for (i = 0; i < BOARD_COL+1; ++i)
        ec_allegro_graphic_line(s, i*BOARD_SIZE, 0, i*BOARD_SIZE, BOARD_HEIGHT, makecol(128, 128, 128));

    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            if (game.board[j][i].building != BUILDING_NONE)
            {
                coord_x = (i*BOARD_SIZE);
                coord_y = (j*BOARD_SIZE);

                ec_allegro_graphic_rectfill(s, coord_x, coord_y, coord_x+BOARD_SIZE, coord_y+BOARD_SIZE, makecol(64, 64, 64));
            }
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
}

void ec_game_render(BITMAP *s)
{
    /* menu */
    rectfill(s, 0, 0, 150, WINDOW_HEIGHT, makecol(200, 200, 200));

    textprintf_ex(s, font, 10, 20, makecol(0, 0, 0), -1, "timer : %d - %d", game.time, game.time/30);
    textprintf_ex(s, font, 10, 40, makecol(0, 0, 0), -1, "%d EceFlouz", game.money);

    textprintf_ex(s, font, 10, 80, makecol(0, 0, 0), -1, "people : %d", game.people);
    textprintf_ex(s, font, 10, 100, makecol(0, 0, 0), -1, "elec : %d/%d", game.elec_used, game.elec_capacity);
    textprintf_ex(s, font, 10, 120, makecol(0, 0, 0), -1, "water : %d/%d", game.water_used, game.water_capacity);
}

