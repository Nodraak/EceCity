
#include "ec_menu.h"


void ec_menu_handle_event(s_menu *menu)
{
    int choix;

    if (window.key[KEY_ESC])
        menu->quit = 1;

    if (window.mouseButtonLeft)
    {
        choix = ec_menu_item_get_hovered(menu, MENU_LAST);

        switch(choix)
        {
            case MENU_MAIN_PLAY:
                menu->quit = 1;
                break;

            case MENU_MAIN_LOAD:
                break;

            case MENU_MAIN_RULES:
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

    nouv = (s_menu*)malloc(sizeof(s_menu));

    if (nouv == NULL)
    {
        printf("ERREUR ALLOCATION MENU"); // ==> Changer par ABORD
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    /* Initialisation des variables */
    nouv->quit = 0;

    //Chargement du background du menu
    nouv->background = ec_utils_load_sprite("main_menu.bmp");

    //Chargement des images du menu

    int i;
    char tmp[1024];
    FILE *fic = NULL;

    fic = fopen("res/menu_img.txt", "r");
    if (fic == NULL)
        ec_utils_abort("fopen() menu_img.txt");


    /* Allocation du tableau de BITMAP du menu */
    nouv->item = NULL;
    nouv->item = (s_menu_item**)malloc(MENU_LAST*sizeof(s_menu_item*));

    if (nouv == NULL)
    {
        printf("ERREUR ALLOCATION MENU"); // ==> Changer par ABORD
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    /* skip header */
    fgets(tmp, 1024-1, fic);
    fgets(tmp, 1024-1, fic);

    for (i = 0; i < MENU_LAST; i++)
    {
        nouv->item[i] = (s_menu_item*)malloc(MENU_LAST*sizeof(s_menu_item));
        if (nouv == NULL)
        {
            printf("ERREUR ALLOCATION MENU"); // ==> Changer par ABORD
            allegro_exit();
            exit(EXIT_FAILURE);
        }

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

int ec_menu_item_get_hovered(s_menu *menu, int nbItem)
{
    int i;

    for (i = 0; i < nbItem; i++)
    {
        if (window.mousePos.x > menu->item[i]->pos.x && window.mousePos.x < menu->item[i]->pos.x+menu->item[i]->sprite->w
            && window.mousePos.y > menu->item[i]->pos.y && window.mousePos.y < menu->item[i]->pos.y+menu->item[i]->sprite->h )
            return i;
    }

    return -1;
}

void ec_menu_render(s_menu *menu)
{
    int i = -1;

    /* clear */
    clear_to_color(window.screen, makecol(255, 255, 255));

    /* draw */
    masked_blit(menu->background, window.screen, 0, 0, 0, 0, menu->background->w, menu->background->h);
    //If item get hovered ==> Draw hovered sprite
    i = ec_menu_item_get_hovered(menu, MENU_LAST);
    if (i != -1)
            draw_sprite(window.screen, menu->item[i]->sprite, menu->item[i]->pos.x, menu->item[i]->pos.y);


    /* flip */
    show_mouse(window.screen);
    //draw_sprite(screen, window.screen, 0, 0);
    blit(window.screen, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Changement pour passer au BLIT

}

void ec_menu_free(s_menu *menu)
{
    int compt;

    for (compt = 0; compt < MENU_LAST; ++compt)
    {
        destroy_bitmap(menu->item[compt]->sprite);
        free(menu->item[compt]);
    }

    free(menu->item);
    destroy_bitmap(menu->background);
    free(menu);

}

void ec_menu_menu(void)
{
    s_menu *menu = NULL;

    menu = ec_menu_load();

    while (!menu->quit)
    {
        ec_allegro_update_event();
        ec_menu_handle_event(menu);
        ec_menu_render(menu);

        rest(1000/WINDOW_FPS);
    }
    ec_menu_free(menu);
}


/*

    todo

    welcome menu + in game menu (maybe split in two files)

    welcome
        new game
        load game
        rules ?
        save current game ?
        delete save

    in game
        resume
        save
        exit

*/
