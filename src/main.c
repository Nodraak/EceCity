
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
            ec_allegro_graphic_line(window.screen, 0, i*BOARD_SIZE, BOARD_WIDTH, i*BOARD_SIZE, makecol(128, 128, 128));
        for (i = 0; i < BOARD_COL+1; ++i)
            ec_allegro_graphic_line(window.screen, i*BOARD_SIZE, 0, i*BOARD_SIZE, BOARD_HEIGHT, makecol(128, 128, 128));

        int coord_x = ec_allegro_graphic_scale_x_pxl_to_coord(window.mousePos.x);
        int coord_y = ec_allegro_graphic_scale_y_pxl_to_coord(window.mousePos.y);

        textprintf_centre_ex(window.screen, font, 100, 100, makecol(0, 0, 0), -1, "%d", coord_x);
        textprintf_centre_ex(window.screen, font, 100, 120, makecol(0, 0, 0), -1, "%d", coord_y);

        if (ec_allegro_graphic_is_in_board(coord_x, coord_y))
        {
            coord_x = coord_x/BOARD_SIZE*BOARD_SIZE;
            coord_y = coord_y/BOARD_SIZE*BOARD_SIZE;

            ec_allegro_graphic_rectfill(window.screen, coord_x, coord_y, coord_x+BOARD_SIZE, coord_y+BOARD_SIZE, makecol(64, 64, 64));
        }

        /* flip */
        show_mouse(window.screen);
        draw_sprite(screen, window.screen, 0, 0);

        rest(1000/WINDOW_FPS);
    }

    ec_allegro_free();

    return EXIT_SUCCESS;
}
