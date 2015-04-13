
/*
    todo

    multiple slots (multiple files) -> required : menu
*/

#include <stdio.h>

#include "ec_allegro.h"
#include "ec_game.h"
#include "ec_building.h"
#include "ec_utils.h"

void ec_save_save(void)
{
    FILE *f = NULL;
    int i, j;

    f = fopen("game.ec", "w");
    if (f == NULL)
        ec_utils_abort("fopen");

    fprintf(f, "%d %d\n", BOARD_LINE, BOARD_COL);

    fprintf(f, "%d %d\n", game.time, game.money);

    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *cur = game.board[j][i];

            if (cur  != NULL)
            {
                if (cur->pos.y == j && cur->pos.x == i)
                {
                    fprintf(f, "BAT %d %d %d %d %d %d %d\n", cur->type, cur->pos.y, cur->pos.x,
                        cur->size.y, cur->size.x, cur->people, cur->evolved);
                }
                else
                    fprintf(f, "PTR\n");
            }
            else
                fprintf(f, "NULL\n");
        }
    }

    fclose(f);
}


void ec_save_load(void)
{
    FILE *f = NULL;
    int i, j, sx, sy;
    char tmp[1024];

    ec_game_init();

    f = fopen("game.ec", "r");
    if (f == NULL)
        ec_utils_abort("fopen");

    if (fgets(tmp, 1023, f) == NULL)
        ec_utils_abort("fgets 1");
    if (sscanf(tmp, "%d %d\n", &j, &i) != 2)
        ec_utils_abort("scanf 1");
    if (i != BOARD_COL || j != BOARD_LINE)
        ec_utils_abort("board size not maching");

    if (fgets(tmp, 1023, f) == NULL)
        ec_utils_abort("fgets 2");
    if (sscanf(tmp, "%d %d\n", &game.time, &game.money) != 2)
        ec_utils_abort("scanf 2");

    game.elec_capacity = 0;
    game.water_capacity = 0;

    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            if (fgets(tmp, 1023, f) == NULL)
                ec_utils_abort("fgets building");

            if (strncmp("BAT", tmp, 3) == 0)
            {
                int x, y, template;
                s_building *new = NULL;

                /* load data */
                if (sscanf(tmp, "BAT %d %d %d", &template, &y, &x) != 3)
                    ec_utils_abort("fscanf building 1");

                new = ec_building_alloc(&building_data[template], y, x);

                if (sscanf(tmp, "BAT %d %d %d %d %d %d %d\n", (int*)&new->type, &new->pos.y, &new->pos.x,
                    &new->size.y, &new->size.x, &new->people, &new->evolved) != 7)
                    ec_utils_abort("fscanf building 2");

                /* save to board */
                game.board[j][i] = new;

                for (sy = 0; sy < new->size.y; ++sy)
                {
                    for (sx = 0; sx < new->size.x; ++sx)
                        game.board[j+sy][i+sx] = new;
                }
            }
            else if (strncmp("PTR", tmp, 3) == 0)
            {
                /* nope */
            }
            else if (strncmp("NULL", tmp, 4) == 0)
                game.board[j][i] = NULL;
            else
                ec_utils_abort("invalid data identifier");
        }
    }

    fclose(f);

    ec_game_on_building_new();
}
