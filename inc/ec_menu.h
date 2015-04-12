#ifndef EC_MENU_H
#define EC_MENU_H

#include <stdlib.h>
#include <stdio.h>

#include "ec_allegro.h"
#include "ec_utils.h"
#include "ec_game.h"
#include "ec_save.h"

typedef enum        _e_menu_item
{
    MENU_MAIN_PLAY = 0,
    MENU_MAIN_LOAD,
    MENU_MAIN_RULES,
    MENU_MAIN_QUIT,

    MENU_PAUSE_RESUME,
    MENU_PAUSE_LOAD,
    MENU_PAUSE_SAVE,
    MENU_PAUSE_MENU,
    MENU_PAUSE_QUIT,

    MENU_LAST
}                   e_menu;


///STRUCTURES


/*      Structure de sprite
Structure contenant un sprite ainsi que sa position
à l'écran
*/
typedef struct _s_menu_item
{
    BITMAP      *sprite;
    s_vector2i  pos;
}               s_menu_item;


/*      Structure MENU
Contient les booléens de test, les arrières-plan du MENU/PAUSE
Contient la bibliothèque d'image "hovered" à afficher quand la souris est sur
une case du MENU ou de la PAUSE
*/
typedef struct _s_menu
{
    char        quit;   /* Booléen pour quitter le MENU */
    char        stop;   /* Booléen pour quitter la PAUSE */

    BITMAP      *background;    /* Arrière-plan du MENU */
    BITMAP      *pause;         /* Arrière-Plan de la PAUSE */
    s_menu_item      **item;    /* Tableau de pointeur sur image ==> Les sprites Hovered */
}       s_menu;


///UTILITAIRE


/*      CHARGEMENT DU MENU
Alloue, Initialise la structure du MENU
Charge les backgrounds
Alloue le tableau de pointeur sur image (COLLECTION DE HOVERED SPRITE)
Alloue et charge dans la structure image le sprite et les coordonnées à l'écran
*/
s_menu *ec_menu_load(void);


/*      Item_get_hovered -- Mouse_is_on_sprite
Paramètre: Collection de sprite, début dans la collection, fin dans la collection.

NOTA:   Les spites hovered ayant exactement les mêmes coordonnées que les cases de choix
        sur le background, Vérifier si la souris est sur le sprite revient à vérifier
        si elle est sur la case de l'écran

Parcourt le tableau de sprite est vérifie si la souris est comprise dans les coordonnées
de l'image. Si c'est le cas ==> retourne la case du sprite correspondant et arreter l'algo
            Sinon ==> Retourne -1
*/
int ec_menu_item_get_hovered(s_menu *menu, int start, int nbItem);


/*      Affichage
Affichage du MENU ou de la PAUSE
utilise ec_menu_item_get_hovered pour vérifier si la souris est sur une image et donc
afficher son HOVERED SPRITE
*/
void ec_menu_render(s_menu *menu, BITMAP *fond, int start, int nbItem);


/*      Libération de la mémoire du menu
Libère la bibliothèque d'image (Bitmap+Structure), les Backgrounds ainsi que la structure MENU
*/
void ec_menu_free(s_menu *menu);


///MENU


/*      Gestion EVENT DU MENU
Vérifie que le clic est bien sur une image
==> Lance l'action associé à l'image cliqué
*/
void ec_menu_handle_event(s_menu *menu);


/*      BOUCLE DU MENU
1°) Update allegro (clavier/souris)
2°) Gestion des actions MENU
3°) Rendu Graphique du MENU
4°) REST (pour l'affichage)
*/
void ec_menu_menu(s_menu *menu);

///PAUSE


/*      GESTION EVENT DE LA PAUSE
Vérifie que le clic est bien sur une image
==> Lance l'action associé à l'image cliqué
*/
void ec_menu_pause_event(s_menu *menu);


/*      BOUCLE DE LA PAUSE
1°) Update allegro (clavier/souris)
2°) Gestion des actions PAUSE
3°) Rendu Graphique de la PAUSE
4°) REST (pour l'affichage)
*/
void ec_menu_handle_pause(s_menu *menu);

#endif
