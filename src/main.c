
#include <stdlib.h>

#include <allegro.h>

#include <ec_allegro.h>
#include <ec_allegro_graphic.h>

int main(int ac, char *av[])
{
    int i;

    (void)ac, (void)av;

    ec_allegro_init();

    while (!window.quit)
    {
        ec_allegro_update_event();
        if (window.key[KEY_ESC])
            window.quit = 1;

        /* clear */
        clear_to_color(window.screen, makecol(255, 255, 255));

        /* draw */

        for (i = 0; i < BOARD_LINE+1; ++i)
            ec_allegro_line(window.screen, 0, i*BOARD_SIZE, BOARD_WIDTH, i*BOARD_SIZE, makecol(128, 128, 128));
        for (i = 0; i < BOARD_COL+1; ++i)
            ec_allegro_line(window.screen, i*BOARD_SIZE, 0, i*BOARD_SIZE, BOARD_HEIGHT, makecol(128, 128, 128));

        /* flip */
        show_mouse(window.screen);
        draw_sprite(screen, window.screen, 0, 0);

        rest(1000/WINDOW_FPS);
    }

    ec_allegro_free();

    return EXIT_SUCCESS;
}
