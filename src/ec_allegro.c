
#include <stdlib.h>
#include <stdio.h>

#include <allegro.h>

#include "ec_allegro.h"
#include "ec_utils.h"
#include "ec_game.h"
#include "ec_graphic.h"

s_window window;

void _ec_allegro_close_button_callback(void)
{
    window.quit = 1;
}

void ec_allegro_init(void)
{
    int i;

    /* alleg */
    allegro_init();

    set_color_depth(desktop_color_depth());
    set_window_title("EceCity");
    if (FULL_SCREEN)
    {
        if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0) != 0)
            ec_utils_abort("set_gfx_mode()");
    }
    else
    {
        if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0) != 0)
            ec_utils_abort("set_gfx_mode()");
    }
    if (set_display_switch_mode(SWITCH_BACKGROUND) != 0)
        ec_utils_abort("set_display_switch_mode()");

    install_keyboard();
    install_mouse();

    set_close_button_callback(_ec_allegro_close_button_callback);

    /* struct */
    window.screen = create_bitmap(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!window.screen)
        ec_utils_abort("create_bitmap() for window.screen");
    window.quit = 0;

    window.mousePos.x = 0;
    window.mousePos.y = 0;
    window.mouseButtonLeft = 0;
    for (i = 0; i < KEY_MAX; ++i)
        window.key[i] = 0;

    window.zoom = 1;
    window.offset.x = 100;
    window.offset.y = -400;

    window.scale_coord_to_pxl = ec_graphic_scale_coord_to_pxl_straight;
    window.scale_pxl_to_coord = ec_graphic_scale_pxl_to_coord_straight;
    window.building_get_sprite = ec_building_get_sprite_straight;
    window.building_get_blit_offset = ec_building_get_blit_offset_straight;
    window.building_get_stat_blit_offset = ec_building_get_stat_blit_offset_straight;
    window.building_get_blit_zoom = ec_building_get_blit_zoom_straight;
}

void ec_allegro_update_event(void)
{
    /* old state */
    static int mouseButtonLeft_old = 0;
    static int key_old[KEY_MAX];
    int i;

    /* mouse pos */
    window.mousePos.x = mouse_x;
    window.mousePos.y = mouse_y;

    /* mouse button left */
    if ((mouse_b & 1) && !mouseButtonLeft_old)
        window.mouseButtonLeft = 1, mouseButtonLeft_old = 1;
    if (!(mouse_b & 1) && mouseButtonLeft_old)
        window.mouseButtonLeft = 0, mouseButtonLeft_old = 0;

    /* key */
    for (i = 0; i < KEY_MAX; ++i)
    {
        if (key[i] && !key_old[i])
            window.key[i] = 1, key_old[i] = 1;
        if (!key[i] && key_old[i])
            window.key[i] = 0, key_old[i] = 0;
    }

    /* zoom */
    if (window.key[KEY_P])
    {
        window.zoom += window.zoom * BOARD_ZOOM_FACTOR;
        window.offset.x -= BOARD_WIDTH/2 / (100 * window.zoom);
        window.offset.y += BOARD_HEIGHT/2  / (100 * window.zoom);
    }
    if (window.key[KEY_M])
    {
        window.zoom -= window.zoom * BOARD_ZOOM_FACTOR;
        window.offset.x += BOARD_WIDTH/2  / (100 * window.zoom);
        window.offset.y -= BOARD_HEIGHT/2  / (100 * window.zoom);
    }

    /*
        move
        ==> Move with Arrows Key or with MOUSE
        ==> Offfset limitation

     */
    if (window.key[KEY_UP] || window.mousePos.y < BOARD_MOVE_MOUSE_AREA)
    {
        if (window.offset.y < BOARD_LIMIT_OFFSET_UP)
            window.offset.y += BOARD_MOVE_SPEED / window.zoom;
    }
    if (window.key[KEY_DOWN] || window.mousePos.y > WINDOW_HEIGHT - BOARD_MOVE_MOUSE_AREA)
    {
        if (window.offset.y > BOARD_LIMIT_OFFSET_DOWN)
            window.offset.y -= BOARD_MOVE_SPEED / window.zoom;
    }

    if (window.key[KEY_RIGHT] || window.mousePos.x > WINDOW_WIDTH - BOARD_MOVE_MOUSE_AREA)
    {
        if (window.offset.x > BOARD_LIMIT_OFFSET_RIGHT)
            window.offset.x -= BOARD_MOVE_SPEED / window.zoom;
    }
    if (window.key[KEY_LEFT] || (window.mousePos.x > TOOLBAR_WIDTH && window.mousePos.x < TOOLBAR_WIDTH + BOARD_MOVE_MOUSE_AREA))
    {
        if (window.offset.x < BOARD_LIMIT_OFFSET_LEFT)
            window.offset.x += BOARD_MOVE_SPEED / window.zoom;
    }
}

void ec_allegro_free(void)
{
    show_mouse(NULL);
    destroy_bitmap(window.screen);

    allegro_exit();
}
