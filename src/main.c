
#include <stdlib.h>

#include <allegro.h>

#include <ec_allegro.h>

int main(int ac, char *av[])
{
    int i;

    (void)ac, (void)av;

    ec_allegro_init();

    while (!window.key[KEY_ESC])
    {
        ec_allegro_update_event();


        /* clear */
        show_mouse(NULL);
        clear_bitmap(window.screen);

        /* draw */
        textprintf_centre_ex(window.screen, font, 100, 100, makecol(255, 255, 255), -1, "%.1f", window.mousePos.x);

        for (i = 0; i < BOARD_LINE+1; ++i)
            ec_allegro_line(window.screen, 0, i*BOARD_SIZE, BOARD_WIDTH, i*BOARD_SIZE, makecol(255, 0, 0));
        for (i = 0; i < BOARD_COL+1; ++i)
            ec_allegro_line(window.screen, i*BOARD_SIZE, 0, i*BOARD_SIZE, BOARD_HEIGHT, makecol(0, 255, 0));


        /* flip */
        show_mouse(window.screen);
        draw_sprite(screen, window.screen, 0, 0);

        rest(1000/WINDOW_FPS);
    }

    ec_allegro_free();

    return EXIT_SUCCESS;
}
