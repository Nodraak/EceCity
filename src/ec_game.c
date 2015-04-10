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
s_toolbar toolbar[TOOLBAR_NB_ICON];

void ec_timer_time_callback(void)
{
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

int ec_game_is_on_sprite(int posx, int posy, int taillex, int tailley)
{
    if (window.mousePos.x > posx && window.mousePos.x < posx+taillex
        && window.mousePos.y > posy && window.mousePos.y < posy+tailley )
        return 1;
    else
        return 0;
}

void ec_game_on_button_left(void)  /* TODO ==> A MODIFIER POUR LA BARRE D'OUTILS */
{
    int pxl_x, pxl_y, board_x, board_y, compt;

    if (window.mousePos.x < 151)
    {
        for (compt = 0; compt < TOOLBAR_NB_BUTTON; compt++)
        {
            s_toolbar *cur = &toolbar[compt];

            if (ec_game_is_on_sprite(cur->pos.x, cur->pos.y, cur->sprite->w, cur->sprite->h))
                break;
        }

        /* todo TEST POUR BINDER LES NIVEAUX -> dafuq ?*/

        switch(compt)
        {
            case 0:
                /* todo A FAIRE SELECTION CASERNE DE POMPIER */
                break;

            case 1:
                game.building_selected = BUILDING_HOUSE_NONE;
                break;

            case 2:
                game.pause = !game.pause;
                break;

            case 3:
                game.building_selected = BUILDING_SUPPLY_ELEC;
                break;

            case 4:
                game.building_selected = BUILDING_INFRA_ROAD;
                break;

            case 5:
                ec_save_save();
                break;

            case 6:
                game.building_selected = BUILDING_SUPPLY_WATER;
                break;

            default:
                game.building_selected = BUILDING_NONE;
                break;
        }

        window.mouseButtonLeft = 0;
    }
    else
    {
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
                ec_building_render(game.board[y][x], x*BOARD_SIZE, y*BOARD_SIZE);
            }
        }
    }

    /* hover */
    if (game.building_selected != BUILDING_NONE)
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
    int i;

    /* menu */
    rectfill(s, 0, 0, 150, WINDOW_HEIGHT-1, makecol(200, 200, 200));

    for (i = 0; i < TOOLBAR_NB_ICON; i++)
        draw_sprite(s, toolbar[i].sprite, toolbar[i].pos.x, toolbar[i].pos.y);

    if (!game.pause)
        draw_sprite(s, toolbar[2].sprite, toolbar[2].pos.x, toolbar[2].pos.y);

    textprintf_ex(s, font, 60, 24, makecol(0, 0, 0), -1, "%ds - %d", game.time, game.time/30);
    textprintf_ex(s, font, 60, 64, makecol(0, 0, 0), -1, "%d", game.money);
    textprintf_ex(s, font, 60, 74, makecol(0, 0, 0), -1, "EceFlouz");

    textprintf_ex(s, font, 60, 119, makecol(0, 0, 0), -1, "%d", game.people);

    textprintf_ex(s, font, 60, 167, makecol(0, 0, 0), -1, "%d/%d", game.elec_used, game.elec_capacity);
    textprintf_ex(s, font, 60, 214, makecol(0, 0, 0), -1, "%d/%d", game.water_used, game.water_capacity);

    /* niveaux */
    rectfill(s, 4, 320, 49, 365, makecol(0, 128, 0));
    textprintf_ex(s, font, 26, 340, makecol(0, 0, 0), -1, "0");

    rectfill(s, 52, 320, 97, 365, makecol(0, 0, 255));
    textprintf_ex(s, font, 69, 340, makecol(0, 0, 0), -1, "-1");

    rectfill(s, 100, 320, 145, 365, makecol(255, 255, 0));
    textprintf_ex(s, font, 117, 340, makecol(0, 0, 0), -1, "-2");

    /* PAUSE */

    if ( game.pause )
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

    /*Skip Info*/
    fgets(tmp, 1024-1, fic);
    fgets(tmp, 1024-1, fic);

    for (i = 0; i < TOOLBAR_NB_ICON; i++)
    {
        nouv = &toolbar[i];

        fgets(tmp, 1024-1, fic);

        fgets(tmp, 1024-1, fic);
        tmp[strlen(tmp)-1] = '\0';
        nouv->sprite = ec_utils_load_sprite(tmp);

        fscanf(fic, "%d %d", &nouv->pos.x, &nouv->pos.y);

        fgets(tmp, 1024-1, fic);
        fgets(tmp, 1024-1, fic);
    }

    fclose(fic);

}

void ec_game_free_toolbar(void)
{
    int compt;

    for( compt = 0; compt < TOOLBAR_NB_ICON; ++compt)
        destroy_bitmap(toolbar[compt].sprite);
}

void ec_game_evolve(void)
{
}
