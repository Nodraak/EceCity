
#include <stdlib.h>
#include <stdio.h>

#include "ec_allegro.h" /* s_vector */
#include "ec_building.h" /* s_building */
#include "ec_graphic.h" /* ec_graphic_is_in_board */
#include "ec_game.h" /* game */
#include "ec_list.h"
#include "ec_graph.h"


void graph_list_add(s_list *list, int x, int y)
{
    s_vector *payload = malloc(sizeof(s_vector));

    payload->x = x,
    payload->y = y;

    list_add(list, payload);
}


s_vector graph_list_ppaf(s_node *node)
{
    s_vector *payload = NULL;
    s_vector ret;

    if (node == NULL || node->payload == NULL)
    {
        printf("Erreur node or payload ... %s %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    payload = node->payload;
    free(node);

    ret.x = payload->x;
    ret.y = payload->y;
    free(payload);

    return ret;
}


void ec_graph_supply_serve(int x, int y)
{
    s_list *list = NULL;
    s_node *node = NULL;
    int available = 0;
    s_building *b = NULL;

    /* init */
    list = list_new();
    graph_list_add(list, 0, 0); /* TODO : centrale->x */
    available = 42; /* TODO : centrale->capacity */

    /* loop */
    while ((node = list_pop(list)) != NULL)
    {
        s_building *b = game.board[y][x];
        s_vector v = graph_list_ppaf(node);

        if (b->type == BUILDING_INFRA_ROAD)
        {
            /* TODO
            for cell in surrounding
                graph_list_add(list, cell);
            */
        }
        else if (ec_building_is_house(b->type))
        {
            /* TODO
            fill house, empty centrale
            */
        }
    }

    /* free */
    list_free(list);
}


int graph_exemple(void)
{
    s_list *list = NULL;
    s_node *node = NULL;

    /* init */
    list = list_new();

    /* push */
    graph_list_add(list, 1, 0);

    /* pop */
    node = list_pop(list);
    if (node != NULL)
    {
        int val = graph_list_ppaf(node).x;
        printf("%d\n", val);
    }

    /* free */
    list_free(list);

    return 0;
}
