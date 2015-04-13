
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

KEY_I : iso
KEY_O : straight

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
        window.key[KEY_L] = 0;
    }

    if (window.key[KEY_I])
    {
        window.scale_coord_to_pxl = ec_graphic_scale_coord_to_pxl_iso;
        window.scale_pxl_to_coord = ec_graphic_scale_pxl_to_coord_iso;
        window.building_get_sprite = ec_building_get_sprite_iso;
        window.building_get_blit_offset = ec_building_get_blit_offset_iso;
        window.building_get_stat_blit_offset = ec_building_get_stat_blit_offset_iso;
        window.building_get_blit_zoom = ec_building_get_blit_zoom_iso;
        window.key[KEY_I] = 0;
    }
    if (window.key[KEY_O])
    {
        window.scale_coord_to_pxl = ec_graphic_scale_coord_to_pxl_straight;
        window.scale_pxl_to_coord = ec_graphic_scale_pxl_to_coord_straight;
        window.building_get_sprite = ec_building_get_sprite_straight;
        window.building_get_blit_offset = ec_building_get_blit_offset_straight;
        window.building_get_stat_blit_offset = ec_building_get_stat_blit_offset_straight;
        window.building_get_blit_zoom = ec_building_get_blit_zoom_straight;
        window.key[KEY_O] = 0;
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
    blit(window.screen, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
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

    /* Chargement des éléments du JEU */
    srand(42);
    ec_allegro_init();
    ec_game_init();
    ec_building_init_all();
    menu = ec_menu_load();

    /* Boucle du Programme */
    while (!window.quit)
    {
        /* Menu */
        ec_menu_menu(menu);

        /* Boncle de Jeu */
        ec_main_gameLoop(menu);
    }

    /* Libération de la mémoire et fermeture du programme */
    ec_building_free_all();
    ec_game_free();
    ec_menu_free(menu);
    ec_allegro_free();

    return EXIT_SUCCESS;
}
END_OF_MAIN()
