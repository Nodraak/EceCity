
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
            if (game.board[j][i] != NULL)
            {
                fprintf(f, "%d %d %d %d %d %d\n", game.board[j][i]->type,
                    game.board[j][i]->pos.y, game.board[j][i]->pos.x,
                    game.board[j][i]->size.y, game.board[j][i]->size.x,
                    game.board[j][i]->people);
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
    int i, j;
    char tmp[1024];

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

            if (strncmp("NULL", tmp, 4) != 0)
            {
                int x, y, template;
                    ec_utils_abort("fscanf building 1");

                if (sscanf(tmp, "%d %d %d", &template, &y, &x) != 3)

                game.board[j][i] = ec_building_alloc(&building_data[template], y, x);
                    ec_utils_abort("fscanf building 2");

                if (sscanf(tmp, "%d %d %d %d %d %d\n", (int*)&game.board[j][i]->type,
                    &game.board[j][i]->pos.y, &game.board[j][i]->pos.x,
                    &game.board[j][i]->size.y, &game.board[j][i]->size.x,
                    &game.board[j][i]->people) != 6)
            }
            else
                game.board[j][i] = NULL;
        }
    }

    fclose(f);
}
