
#include <stdlib.h>
#include <stdio.h>

#include <allegro.h>

#include <ec_allegro.h>

s_window window;

void _ec_abort(char *error, char *file, int line)
{
    char msg[1024];

    sprintf(msg, "Error !\nLine %d in %s\n%s", line, &file[4], error);
    allegro_message(msg);
    allegro_exit();
    exit(EXIT_FAILURE);
}

void ec_allegro_init(void)
{
    int i;

    /* alleg */
    allegro_init();

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0) != 0)
        ec_abort("set_gfx_mode()");

    install_keyboard();
    install_mouse();

    /* struct */
    window.screen = create_bitmap(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!window.screen)
        ec_abort("create_bitmap() for window.screen");
    window.quit = 0;

    window.mousePos.x = 0;
    window.mousePos.y = 0;
    window.mouseButtonLeft = 0;
    for (i = 0; i < KEY_MAX; ++i)
        window.key[i] = 0;

    window.zoom = 1;
    window.offset.x = 0;
    window.offset.y = 0;
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
        window.zoom += window.zoom * 0.02;
        window.offset.x -= BOARD_WIDTH/100 / window.zoom;
        window.offset.y -= BOARD_HEIGHT/100 / window.zoom;
    }
    if (window.key[KEY_M])
    {
        window.zoom -= window.zoom * 0.02;
        window.offset.x += BOARD_WIDTH/100 / window.zoom;
        window.offset.y += BOARD_HEIGHT/100 / window.zoom;
    }

    /* move */
    if (window.key[KEY_UP])
        window.offset.y += 4 / window.zoom;
    if (window.key[KEY_DOWN])
        window.offset.y -= 4 / window.zoom;

    if (window.key[KEY_RIGHT])
        window.offset.x -= 4 / window.zoom;
    if (window.key[KEY_LEFT])
        window.offset.x += 4 / window.zoom;
}

void ec_allegro_free(void)
{
    show_mouse(NULL);
    destroy_bitmap(window.screen);

    allegro_exit();
}

/******************************************************************************/

int _ec_allegro_scale(int window_max, double window_offset, int val)
{
    double coord_min = 0 - window_offset;
    double coord_max = window_max/window.zoom - window_offset;
    int pxl_min = 0;
    int pxl_max = window_max;

    return pxl_min + (val-coord_min) * (pxl_max-pxl_min) / (coord_max-coord_min);
}

int ec_allegro_scale_x_coord_to_pxl(int val)
{
    return _ec_allegro_scale(WINDOW_WIDTH, window.offset.x, val);
}

int ec_allegro_scale_y_coord_to_pxl(int val)
{
    return _ec_allegro_scale(WINDOW_HEIGHT, window.offset.y, val);
}

void ec_allegro_line(BITMAP *screen, int x1, int y1, int x2, int y2, int c)
{
    x1 = ec_allegro_scale_x_coord_to_pxl(x1);
    y1 = ec_allegro_scale_y_coord_to_pxl(y1);
    x2 = ec_allegro_scale_x_coord_to_pxl(x2);
    y2 = ec_allegro_scale_y_coord_to_pxl(y2);

    line(screen, x1, y1, x2, y2, c);
}

