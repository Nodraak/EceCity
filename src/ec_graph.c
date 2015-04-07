
#include <stdlib.h>
#include <stdio.h>

#include "ec_building.h" /* s_building */
#include "ec_graphic.h" /* ec_graphic_is_in_board */
#include "ec_game.h" /* game */
#include "ec_list.h"
#include "ec_graph.h"


void graph_list_add(s_list *list, int x, int y)
{
    s_vector2i *payload = ec_utils_malloc(sizeof(s_vector2i));

    payload->x = x,
    payload->y = y;

    list_add(list, payload);
}


void graph_list_free_node(s_node *node)
{
    if (node == NULL || node->payload == NULL)
    {
        printf("Erreur node or payload ... %s %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    free(node->payload);
    free(node);
}


s_vector2i graph_list_get_vector2i(s_node *node)
{
    if (node == NULL || node->payload == NULL)
    {
        printf("Erreur node or payload ... %s %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    return *(s_vector2i*)node->payload;
}


void graph_list_add_surrouding(s_list *list, s_vector2i v)
{
    list_add(list, ec_utils_vector2i_alloc(v.x+1, v.y));
    list_add(list, ec_utils_vector2i_alloc(v.x-1, v.y));
    list_add(list, ec_utils_vector2i_alloc(v.x, v.y+1));
    list_add(list, ec_utils_vector2i_alloc(v.x, v.y-1));
}


int ec_graph_supply_building(s_list *list, int *available, s_vector2i pos, s_ressource*(*get_resrc)(s_building *b))
{
    int served = 0;
    s_building *b = game.board[pos.y][pos.x];

    /* if null, return */
    if (b == NULL || get_resrc(b)->visited)
        return 0;

    get_resrc(b)->visited = 1;

    /* road, recurs */
    if (b->type == BUILDING_INFRA_ROAD)
    {
        graph_list_add_surrouding(list, pos);
    }
    /* house, serve */
    else if (ec_building_is_house(b->type))
    {
        int needed = get_resrc(&building_data[b->type])->used - get_resrc(b)->used;

        if (needed > 0)
        {
            served = (needed < (*available) ? needed : (*available));
            get_resrc(b)->used += served;
            *available -= served;
        }
    }

    return served;
}


int ec_graph_supply_board(s_building *b, s_vector2i pos, s_ressource*(*get_resrc)(s_building *b))
{
    s_list *list = NULL;
    s_node *node = NULL;
    int available = 0, served = 0, i, j;

    /* init */
    for (j = 0; j < BOARD_LINE; ++j)
    {
        for (i = 0; i < BOARD_COL; ++i)
        {
            s_building *b = game.board[j][i];

            if (b != NULL)
                get_resrc(b)->visited = 0;
        }
    }

    list = list_new();
    graph_list_add_surrouding(list, pos);
    available = get_resrc(b)->produced;

    /* loop */
    while ((node = list_pop(list)) != NULL)
    {
        s_vector2i cur_pos = graph_list_get_vector2i(node);

        if (ec_utils_pxl_is_in_board(cur_pos.x*BOARD_SIZE, cur_pos.y*BOARD_SIZE))
            served += ec_graph_supply_building(list, &available, cur_pos, get_resrc);

        graph_list_free_node(node);
    }

    get_resrc(b)->produced = available;

    /* free */
    list_free(list);

    return served;
}
