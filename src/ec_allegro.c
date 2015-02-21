
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
    show_mouse(screen);

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
