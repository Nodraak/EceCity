
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <allegro.h>

#include "ec_allegro.h"
#include "ec_allegro_graphic.h"
#include "ec_game.h"


void ec_main_handle_event(void)
{
    if (window.key[KEY_ESC])
        window.quit = 1;

    if (window.mouseButtonLeft)
        ec_game_on_button_left();
}

void ec_main_render(void)
{
    /* clear */
    clear_to_color(window.screen, makecol(255, 255, 255));

    /* draw */
    ec_board_render(window.screen);
    ec_game_render(window.screen);

    /* flip */
    show_mouse(window.screen);
    draw_sprite(screen, window.screen, 0, 0);
}

int main(void)
{
    ec_allegro_init();
    ec_game_init();

    while (!window.quit)
    {
        ec_allegro_update_event();
        ec_main_handle_event();
        ec_main_render();

        rest(1000/WINDOW_FPS);
    }

    ec_allegro_free();

    return EXIT_SUCCESS;
}
