
#include "ec_menu.h"

void ec_menu_handle_event(s_menu *menu)
{
    int choix;

    if (window.mouseButtonLeft)
    {
        //Vérification du clic souris sur une image
        choix = ec_menu_item_get_hovered(menu, MENU_MAIN_PLAY, MENU_PAUSE_RESUME);

        switch(choix)
        {
            case MENU_MAIN_PLAY:
                ec_game_init();
                menu->quit = 1;
                break;

            case MENU_MAIN_LOAD:
                ec_save_load();
                menu->quit = 1;
                break;

            case MENU_MAIN_RULES:
                allegro_message("\t   RULES\n\n\nBienvenue a vous maire de ECE-city\nMission: Faire prosperer votre ville\nchoississez votre mode de jeu..... a completer");
                break;

            case MENU_MAIN_QUIT:
                window.quit = 1;
                game.quit = 1;
                menu->quit = 1;
                break;

            default:
                break;
        }
        window.mouseButtonLeft = 0;
    }

}

s_menu *ec_menu_load(void)
{
    s_menu *nouv = NULL;

    nouv = malloc(sizeof(s_menu));
    if (nouv == NULL)
        ec_utils_abort("allocation menu");

    /* Initialisation des booléens du menu */
    nouv->quit = 0;
    nouv->stop = 0;

    //Chargement du background du menu
    nouv->background = ec_utils_load_sprite("main_menu.bmp");
    //Chargement du background de la PAUSE
    nouv->pause = ec_utils_load_sprite("background_pause.bmp");

    //Chargement des images du menu
    int i;
    char tmp[1024];
    FILE *fic = NULL;

    fic = fopen("res/menu_img.txt", "r");
    if (fic == NULL)
        ec_utils_abort("fopen() menu_img.txt");

    /* Allocation du tableau de BITMAP du menu */
    nouv->item = malloc(MENU_LAST*sizeof(s_menu_item*));
    if (nouv->item == NULL)
        ec_utils_abort("allocation item*");

    /* skip header */
    fgets(tmp, 1024-1, fic);
    fgets(tmp, 1024-1, fic);

    for (i = 0; i < MENU_LAST; i++)
    {
        nouv->item[i] = malloc(MENU_LAST*sizeof(s_menu_item));
        if (nouv->item[i] == NULL)
            ec_utils_abort("allocation item");

        fgets(tmp, 1024-1, fic);

        fgets(tmp, 1024-1, fic);
        tmp[strlen(tmp)-1] = '\0';
        if (strlen(tmp) != 0)
            nouv->item[i]->sprite = ec_utils_load_sprite(tmp);

        fgets(tmp, 1024-1, fic);
        sscanf(tmp, "%d %d", &nouv->item[i]->pos.x, &nouv->item[i]->pos.y);

        fgets(tmp, 1024-1, fic);
    }

    fclose(fic);

    return nouv;
}

int ec_menu_item_get_hovered(s_menu *menu, int start, int nbItem)
{
    int i;

    for (i = start; i < nbItem; i++)
    {
        s_menu_item *cur = menu->item[i];

        if (window.mousePos.x > cur->pos.x && window.mousePos.x < cur->pos.x+cur->sprite->w
            && window.mousePos.y > cur->pos.y && window.mousePos.y < cur->pos.y+cur->sprite->h)
            return i;
    }

    return -1;
}

void ec_menu_render(s_menu *menu, BITMAP *fond, int start, int nbItem)
{
    int i = -1;

    /* clear */
    clear_to_color(window.screen, makecol(255, 255, 255));

    /* draw */
    masked_blit(fond, window.screen, 0, 0, 0, 0, fond->w, fond->h);
    //If item get hovered ==> Draw hovered sprite
    i = ec_menu_item_get_hovered(menu, start, nbItem);
    if (i != -1)
        draw_sprite(window.screen, menu->item[i]->sprite, menu->item[i]->pos.x, menu->item[i]->pos.y);

    /* flip */
    show_mouse(window.screen);
    blit(window.screen, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Changement pour passer au BLIT

}

void ec_menu_free(s_menu *menu)
{
    int compt;

    /* Libération des images hovered du MENU et de la PAUSE*/
    for (compt = 0; compt < MENU_LAST; ++compt)
    {
        destroy_bitmap(menu->item[compt]->sprite);
        free(menu->item[compt]);
    }

    /* Libération de la strcuture du MENU et des images d'arrière plan */
    free(menu->item);
    destroy_bitmap(menu->background);
    destroy_bitmap(menu->pause);
    free(menu);

}


void ec_menu_menu(s_menu *menu)
{
    while (!menu->quit)
    {
        ec_allegro_update_event();
        ec_menu_handle_event(menu);
        ec_menu_render(menu, menu->background, MENU_MAIN_PLAY, MENU_PAUSE_RESUME);

        rest(1000/WINDOW_FPS);
    }

    menu->quit = 0;
}

void ec_menu_pause_event(s_menu *menu)
{
    int choix;

    if (window.mouseButtonLeft)
    {
        //Vérification du clic souris sur une image
        choix = ec_menu_item_get_hovered(menu, MENU_PAUSE_RESUME, MENU_LAST);

        switch(choix)
        {
            case MENU_PAUSE_RESUME:
                menu->stop = 1;
                break;

            case MENU_PAUSE_LOAD:
                ec_save_load();
                menu->stop = 1;
                break;

            case MENU_PAUSE_SAVE:
                ec_save_save();
                break;

            case MENU_PAUSE_MENU:
                menu->stop = 1;
                game.quit = 1;
                break;

            case MENU_PAUSE_QUIT:
                menu->stop = 1;
                game.quit = 1;
                window.quit = 1;
                break;

            default:
                break;
        }
        window.mouseButtonLeft = 0;
    }

    //PROBLEME: ==> RENDRE DEDANS IMMEDIATEMENT APRES LE 1ER ECHAP
    /*if (window.key[KEY_ESC])
    {
        menu->stop = 1;
    }*/

}


void ec_menu_handle_pause(s_menu *menu)
{
    while (!menu->stop)
    {
        ec_allegro_update_event();
        ec_menu_pause_event(menu);
        ec_menu_render(menu, menu->pause, MENU_PAUSE_RESUME, MENU_LAST);

        rest(1000/WINDOW_FPS);
    }

    menu->stop = 0;
}
