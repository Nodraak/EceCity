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

    /* Initialisation des booléens de jeu */
    game.pause = 0;
    game.quit = 0;

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
            s_vector2d pxl;
            s_vector2i board;

            pxl = window.scale_pxl_to_coord(window.mousePos);
            board.x = pxl.x/BOARD_SIZE;
            board.y = pxl.y/BOARD_SIZE;

            if (ec_utils_pxl_is_in_board(pxl.x, pxl.y)
                && ec_building_have_space(board.y, board.x, building_data[game.building_selected].size)
                && game.building_selected != BUILDING_NONE
                && game.money >= building_data[game.building_selected].price)
            {
                ec_building_new(board.y, board.x, game.building_selected);
                ec_game_on_building_new();
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
                else
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
                s_vector2i pos = ec_utils_vector2i_make(i, j);

                if (b->type == BUILDING_SUPPLY_WATER)
                    game.water_used += ec_graph_supply_board(b, pos, ec_building_resrc_get_water);
                else if (b->type == BUILDING_SUPPLY_ELEC)
                    game.elec_used += ec_graph_supply_board(b, pos, ec_building_resrc_get_elec);
            }
        }
    }

    /* update nb_people */
    game.people = 0;

    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *b = game.board[j][i];

            if (b != NULL && ec_building_is_house(b->type)
                && game.board[j][i]->pos.x == i && game.board[j][i]->pos.y == j)
            {
                if (b->elec.used == building_data[b->type].elec.used)
                {
                    game.people += b->water.used;
                }
                else
                    b->elec.used = 0;
            }
        }
    }
}


void ec_game_render_board(BITMAP *s)
{
    int i, j;

    s_vector2d v1 = ec_utils_vector2d_make(0, 0);
    s_vector2d v2 = ec_utils_vector2d_make(0, BOARD_LINE*BOARD_SIZE);
    s_vector2d v3 = ec_utils_vector2d_make(BOARD_COL*BOARD_SIZE, BOARD_LINE*BOARD_SIZE);
    s_vector2d v4 = ec_utils_vector2d_make(BOARD_COL*BOARD_SIZE, 0);

    ec_graphic_polygon(s, v1, v2, v3, v4, makecol(50, 160, 50));

    int c = makecol(128, 128, 128);
    for (i = 0; i < BOARD_LINE+1; ++i)
        ec_graphic_line(s, 0, i*BOARD_SIZE, BOARD_WIDTH, i*BOARD_SIZE, c);
    for (i = 0; i < BOARD_COL+1; ++i)
        ec_graphic_line(s, i*BOARD_SIZE, 0, i*BOARD_SIZE, BOARD_HEIGHT, c);

    /* board (buildings) */
    for (i = -BOARD_LINE+1; i < BOARD_COL; ++i)
    {
        for (j = 0; j < BOARD_LINE; ++j)
        {
            int x = i+j;
            int y = j;

            /* building */
            if (ec_utils_cell_is_in_board(x, y) && game.board[y][x] != NULL
                && game.board[y][x]->pos.x == x && game.board[y][x]->pos.y == y)
            {
                ec_building_render(window.screen, game.board[y][x], x, y);
            }

            /* hover */
            s_vector2d tmp = window.scale_pxl_to_coord(window.mousePos);
            s_vector2i coord = ec_utils_vector2i_make(tmp.x, tmp.y);
            coord.x = (coord.x/BOARD_SIZE)*BOARD_SIZE;
            coord.y = (coord.y/BOARD_SIZE)*BOARD_SIZE;

            if (coord.x/BOARD_SIZE == x && coord.y/BOARD_SIZE == y)
            {
                if (game.layer == 0 && game.building_selected != BUILDING_NONE)
                {
                    if (ec_utils_pxl_is_in_board(coord.x, coord.y))
                    {
                        int c = 0;
                        int w = building_data[game.building_selected].size.x*BOARD_SIZE;
                        int h = building_data[game.building_selected].size.y*BOARD_SIZE;
                        s_vector2d v1 = ec_utils_vector2d_make(coord.x,     coord.y);
                        s_vector2d v2 = ec_utils_vector2d_make(coord.x,     coord.y+h);
                        s_vector2d v3 = ec_utils_vector2d_make(coord.x+w,   coord.y+h);
                        s_vector2d v4 = ec_utils_vector2d_make(coord.x+w,   coord.y);

                        if (ec_building_have_space(coord.y/BOARD_SIZE, coord.x/BOARD_SIZE, building_data[game.building_selected].size))
                        {
                            c = makecol(150, 150, 150);
                            ec_graphic_polygon(s, v1, v2, v3, v4, c);

                            ec_graphic_stretch_sprite(s, &building_data[game.building_selected], coord.x, coord.y);
                        }
                        else
                        {
                            c = makecol(230, 50, 50);
                            ec_graphic_polygon(s, v1, v2, v3, v4, c);
                        }
                    }
                }
            }
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


int count_missing_resrc(void)
{
    int i, j;
    int missing_resrc = 0;

    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *cur = game.board[j][i];

            if (cur != NULL && ec_building_is_house(cur->type))
            {
                if (cur->water.used != building_data[cur->type].water.used || cur->elec.used != building_data[cur->type].elec.used)
                    missing_resrc += building_data[cur->type].water.used - cur->water.used;
            }
        }
    }

    return missing_resrc;
}

int peut_etre_pas_de_bol(void)
{
    return (rand() % 40) == 0;
}

void cest_le_jeu_ma_pauvre_lucette(int i, int j)
{
    int x, y;
    s_building *cur = game.board[j][i];

    if (cur->is_burning)
    {
        if (game.time - cur->is_burning > 30)
        {
            for (x = 0; x < cur->size.x; ++x)
            {
                for (y = 0; y < cur->size.y; ++y)
                {
                    game.board[j+y][i+x] = NULL;
                }
            }
        }

        free(cur);
    }
}

void ec_game_evolve(void)
{
    int i, j, evolved = 0;

    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *cur = game.board[j][i];

            if (cur != NULL && ec_building_is_house(cur->type) && cur->evolved + BUILDING_EVOLVE_DELAY < game.time)
            {
                /* fire */
                if (peut_etre_pas_de_bol() && !game.board[j][i]->is_burning)
                    game.board[j][i]->is_burning = game.time;
                cest_le_jeu_ma_pauvre_lucette(i, j);

                /* downgrade */
                if (cur->water.used != building_data[cur->type].water.used || cur->elec.used != building_data[cur->type].elec.used)
                {
                    game.money += cur->water.used * TAX_PER_INHABITANT;
                    evolved += ec_building_evolve(cur, -1);
                }
                /* upgrade */
                else
                {
                    if (game.mode == GAME_MODE_CAPITALIST)
                    {
                        game.money += cur->water.used * TAX_PER_INHABITANT;
                        evolved += ec_building_evolve(cur, 1);
                    }
                    else if (game.mode == GAME_MODE_COMMUNIST)
                    {
                        int tax = cur->water.used * TAX_PER_INHABITANT;

                        evolved += ec_building_evolve(cur, 1);

                        if (count_missing_resrc() > 0)      /* revert */
                            evolved -= ec_building_evolve(cur, -1);
                        else                                /* commit */
                            game.money += tax;
                    }
                }
            }
        }
    }

    if (evolved)
        ec_game_on_building_new();
}
