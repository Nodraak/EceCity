
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <allegro.h>

#include "ec_allegro.h"
#include "ec_graphic.h"
#include "ec_game.h"
#include "ec_save.h"
#include "ec_menu.h"

/*

KEY_ESC : exit
ARROWS : move the board
KEY_P / KEY_M : zoom
KEY_N : next building
KEY_S : save
KEY_L : load

img :
    rotate = 45
    x*y = 346*200

*/

void ec_main_handle_event(s_menu *menu)
{
    if (window.key[KEY_ESC])
    {
        ec_menu_handle_pause(menu);
    }

    if (window.key[KEY_N])
    {
        game.building_selected = (game.building_selected+1) % BUILDING_LAST;
        window.key[KEY_N] = 0;
    }

    if (window.key[KEY_S])
    {
        ec_save_save();
        window.key[KEY_S] = 0;
    }

    if (window.key[KEY_L])
    {
        ec_save_load();
        ec_game_on_building_new();
        window.key[KEY_L] = 0;
    }

    if (window.mouseButtonLeft)
        ec_game_on_button_left();

    ec_game_evolve();
}

void ec_main_render(void)
{
    /* clear */
    clear_to_color(window.screen, makecol(255, 255, 255));

    /* draw */
    ec_game_render_board(window.screen);
    ec_game_render_menu(window.screen);

    /* flip */
    show_mouse(window.screen);
    //draw_sprite(screen, window.screen, 0, 0);
    blit(window.screen, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Changement pour passer au BLIT
}

void ec_main_gameLoop(s_menu *menu)
{
    while(!game.quit)
    {
        ec_allegro_update_event();
        ec_main_handle_event(menu);
        ec_main_render();

        rest(1000/WINDOW_FPS);
    }
    game.quit = 0;
}

int main(void)
{
    s_menu *menu = NULL;

    ec_allegro_init();
    ec_game_init();
    ec_building_init_all();

    menu = ec_menu_load();

    while (!window.quit)
    {
        ec_menu_menu(menu);

        ec_main_gameLoop(menu);
    }

    ec_building_free_all();
    ec_game_free();
    ec_menu_free(menu);
    ec_allegro_free();

    return EXIT_SUCCESS;
}
END_OF_MAIN()
