
#include <stdlib.h>
#include <stdio.h>

#include <allegro.h>

#include <ec_allegro.h>

s_allegro window;

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
    allegro_init();

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0) != 0)
        ec_abort("set_gfx_mode()");

    install_keyboard();
    install_mouse();

    window.screen = create_bitmap(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!window.screen)
        ec_abort("create_bitmap() for window.screen");
    window.quit = 0;
    window.mousePos.x = 0;
    window.mousePos.y = 0;
    window.mouseRel.x = 0;
    window.mouseRel.y = 0;
    window.mouseButtonLeft = 0;
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

    get_mouse_mickeys(&window.mouseRel.x, &window.mouseRel.y);

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
}

void ec_allegro_free(void)
{
    show_mouse(NULL);
    destroy_bitmap(window.screen);

    allegro_exit();
}
