/* TO DO LIST TOOLBAR */

/*
==> Afficher les noms sous les icones
==>Refaire les rectangle pour les niveau
==>Refaire le fond de la barre
==> Meilleur icone de route??
==> Quand le souris dans la barre de tache retirer la prévisualisation??
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ec_game.h"
#include "ec_graphic.h"
#include "ec_building.h"
#include "ec_graph.h"
#include "ec_save.h"
#include "ec_utils.h"

s_game game;
s_toolbar toolbar[TOOLBAR_LAST];

void ec_timer_time_callback(void)
{
    if (!game.pause)
        game.time += 1;
}

void ec_game_init(void)
{
    memset(&game, 0, sizeof(s_game));
    game.money = 500000;

    install_int(ec_timer_time_callback, 1000);

    ec_game_load_toolbar();
}

void ec_game_free(void)
{
    int i, j;

    /* board */
    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *b = game.board[j][i];

            if (b != NULL)
            {
                if (b->pos.x == i && b->pos.y == j)
                    free(b);
            }
        }
    }

    /* toolbar */
    ec_game_free_toolbar();
}

int ec_game_toolbar_get_hovered_button(void)
{
    int i;

    for (i = 0; i < TOOLBAR_LAST; i++)
    {
        s_toolbar *cur = &toolbar[i];

        if (window.mousePos.x > cur->pos.x && window.mousePos.x < cur->pos.x+cur->sprite->w
            && window.mousePos.y > cur->pos.y && window.mousePos.y < cur->pos.y+cur->sprite->h )
            return i;
    }

    return -1;
}

void ec_game_toolbar_handle_clicked_button(int button)
{
    switch(button)
    {
        case TOOLBAR_SAVE:
            ec_save_save();
            break;

        case TOOLBAR_PAUSE:
        case TOOLBAR_PLAY:
            game.pause = !game.pause;
            break;

        case TOOLBAR_LEVEL_0:
        case TOOLBAR_LEVEL_MINUS_1:
        case TOOLBAR_LEVEL_MINUS_2:
            game.layer = button - TOOLBAR_LEVEL_0;
            break;

        case TOOLBAR_BUILDING_ROAD:
            game.building_selected = BUILDING_INFRA_ROAD;
            break;

        case TOOLBAR_BUILDING_HOUSE:
            game.building_selected = BUILDING_HOUSE_NONE;
            break;

        case TOOLBAR_BUILDING_ELEC:
            game.building_selected = BUILDING_SUPPLY_ELEC;
            break;

        case TOOLBAR_BUILDING_WATER:
            game.building_selected = BUILDING_SUPPLY_WATER;
            break;

        case TOOLBAR_LAST:
            printf("Error, case not handled.\n");
            break;

        default:
            printf("default %d\n", button);
            game.building_selected = BUILDING_NONE;
            break;
    }
}

void ec_game_on_button_left(void)
{
    if (window.mousePos.x < TOOLBAR_WIDTH)
    {
        int button = -1;

        button = ec_game_toolbar_get_hovered_button();
        ec_game_toolbar_handle_clicked_button(button);

        window.mouseButtonLeft = 0;
    }
    else
    {
        if (game.layer == 0)
        {
            int pxl_x, pxl_y, board_x, board_y;

            pxl_x = ec_graphic_scale_x_pxl_to_coord(window.mousePos);
            pxl_y = ec_graphic_scale_y_pxl_to_coord(window.mousePos);
            board_x = pxl_x/BOARD_SIZE;
            board_y = pxl_y/BOARD_SIZE;

            if (ec_utils_pxl_is_in_board(pxl_x, pxl_y)
                && ec_building_have_space(board_y, board_x, building_data[game.building_selected].size)
                && game.building_selected != BUILDING_NONE
                && game.money >= building_data[game.building_selected].price)
            {
                ec_building_new(board_y, board_x);
            }

            window.mouseButtonLeft = 0;
        }
    }
}

void ec_game_on_building_new(void)
{
    int i, j;

    /* reset */
    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *b = game.board[j][i];

            if (b != NULL)
            {
                if (ec_building_is_house(b->type))
                {
                    b->elec.used = 0;
                    b->water.used = 0;
                }
                else /* if supply */
                {
                    b->elec.produced = building_data[b->type].elec.produced;
                    b->water.produced = building_data[b->type].water.produced;
                }
            }
        }
    }

    game.water_used = 0;
    game.elec_used = 0;

    /* serve water + elec */
    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *b = game.board[j][i];

            if (b != NULL)
            {
                if (b->type == BUILDING_SUPPLY_WATER)
                    game.water_used += ec_graph_supply_board(b, ec_utils_vector2i_make(i, j), ec_building_resrc_get_water);
                else if (b->type == BUILDING_SUPPLY_ELEC)
                    game.elec_used += ec_graph_supply_board(b, ec_utils_vector2i_make(i, j), ec_building_resrc_get_elec);
            }
        }
    }

    /* update is_working */
    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *b = game.board[j][i];

            if (b != NULL && ec_building_is_house(b->type))
            {
                if (b->water.used == building_data[b->type].water.used && b->elec.used == building_data[b->type].elec.used)
                    b->is_working = 1;
                else
                    b->is_working = 0;
            }
        }
    }
}


void ec_game_render_board(BITMAP *s)
{
    int i, j;

    int vertices[4*2];
    s_vector2i v1 = ec_graphic_scale_coord_to_pxl(ec_utils_vector2d_make(0, 0));
    s_vector2i v2 = ec_graphic_scale_coord_to_pxl(ec_utils_vector2d_make(0, BOARD_LINE*BOARD_SIZE));
    s_vector2i v3 = ec_graphic_scale_coord_to_pxl(ec_utils_vector2d_make(BOARD_COL*BOARD_SIZE, BOARD_LINE*BOARD_SIZE));
    s_vector2i v4 = ec_graphic_scale_coord_to_pxl(ec_utils_vector2d_make(BOARD_COL*BOARD_SIZE, 0));
    vertices[0] = v1.x;
    vertices[1] = v1.y;
    vertices[2] = v2.x;
    vertices[3] = v2.y;
    vertices[4] = v3.x;
    vertices[5] = v3.y;
    vertices[6] = v4.x;
    vertices[7] = v4.y;

    polygon(s, 4, vertices, makecol(230, 230, 230));

    for (j = 0; j < BOARD_LINE+1; ++j)
    {
        for (i = 0; i < BOARD_COL+1; ++i)
            ec_graphic_putpixel(s, i*BOARD_SIZE, j*BOARD_SIZE, makecol(128, 128, 128));
    }

    /* board (buildings) */
    for (i = -BOARD_LINE+1; i < BOARD_COL; ++i)
    {
        for (j = 0; j < BOARD_LINE; ++j)
        {
            int x = i+j;
            int y = j;

            if (ec_utils_cell_is_in_board(x, y) && game.board[y][x] != NULL
                && game.board[y][x]->pos.x == x && game.board[y][x]->pos.y == y)
            {
                ec_building_render(window.screen, game.board[y][x], x*BOARD_SIZE, y*BOARD_SIZE, x, y);
            }
        }
    }

    /* hover */
    if (game.layer == 0 && game.building_selected != BUILDING_NONE)
    {
        int coord_x = ec_graphic_scale_x_pxl_to_coord(window.mousePos);
        int coord_y = ec_graphic_scale_y_pxl_to_coord(window.mousePos);

        if (ec_utils_pxl_is_in_board(coord_x, coord_y))
        {
            coord_x = coord_x/BOARD_SIZE*BOARD_SIZE;
            coord_y = coord_y/BOARD_SIZE*BOARD_SIZE;

            ec_graphic_stretch_sprite(s, &building_data[game.building_selected], coord_x, coord_y);
        }
    }
}

void ec_game_render_menu(BITMAP *s)
{
    int i, minu, sec;

    /* time */
    minu = game.time/60;
    sec = game.time%60;

    /* menu */
    rectfill(s, 0, 0, TOOLBAR_WIDTH, WINDOW_HEIGHT, makecol(200, 200, 200));

    for (i = 0; i < TOOLBAR_LAST; i++)
        draw_sprite(s, toolbar[i].sprite, toolbar[i].pos.x, toolbar[i].pos.y);

    if (game.pause)
        draw_sprite(s, toolbar[TOOLBAR_PLAY].sprite, toolbar[TOOLBAR_PLAY].pos.x, toolbar[TOOLBAR_PLAY].pos.y);
    else
        draw_sprite(s, toolbar[TOOLBAR_PAUSE].sprite, toolbar[TOOLBAR_PAUSE].pos.x, toolbar[TOOLBAR_PAUSE].pos.y);

    /* text */
    if (minu == 0)
        textprintf_ex(s, font, 60, 24, makecol(0, 0, 0), -1, "%ds", sec);
    else
        textprintf_ex(s, font, 60, 24, makecol(0, 0, 0), -1, "%dmin %ds", minu, sec);

    textprintf_ex(s, font, 60, 64, makecol(0, 0, 0), -1, "%d", game.money);
    textprintf_ex(s, font, 60, 74, makecol(0, 0, 0), -1, "EceFlouz");

    textprintf_ex(s, font, 60, 119, makecol(0, 0, 0), -1, "%d", game.people);

    textprintf_ex(s, font, 60, 167, makecol(0, 0, 0), -1, "%d/%d", game.elec_used, game.elec_capacity);
    textprintf_ex(s, font, 60, 214, makecol(0, 0, 0), -1, "%d/%d", game.water_used, game.water_capacity);

    textprintf_ex(s, font, 25, 340, makecol(0, 0, 0), -1, "0");
    textprintf_ex(s, font, 65, 340, makecol(0, 0, 0), -1, "-1");
    textprintf_ex(s, font, 115, 340, makecol(0, 0, 0), -1, "-2");

    if (game.pause)
        textprintf_ex(s, font, 512, 5, makecol(255, 0, 0), -1, "JEU EN PAUSE");
}

void ec_game_load_toolbar(void)
{
    int i;
    char tmp[1024];
    s_toolbar *nouv = NULL;
    FILE *fic = NULL;

    fic = fopen("res/toolbar_img.txt", "r");
    if (fic == NULL)
        ec_utils_abort("fopen() toolbar_img.txt");

    /* skip header */
    fgets(tmp, 1024-1, fic);
    fgets(tmp, 1024-1, fic);

    for (i = 0; i < TOOLBAR_LAST; i++)
    {
        nouv = &toolbar[i];

        fgets(tmp, 1024-1, fic);

        fgets(tmp, 1024-1, fic);
        tmp[strlen(tmp)-1] = '\0';
        if (strlen(tmp) != 0)
            nouv->sprite = ec_utils_load_sprite(tmp);

        fgets(tmp, 1024-1, fic);
        sscanf(tmp, "%d %d", &nouv->pos.x, &nouv->pos.y);

        fgets(tmp, 1024-1, fic);
    }

    fclose(fic);
}

void ec_game_free_toolbar(void)
{
    int compt;

    for (compt = 0; compt < TOOLBAR_LAST; ++compt)
        destroy_bitmap(toolbar[compt].sprite);
}

void ec_game_evolve(void)
{
}
