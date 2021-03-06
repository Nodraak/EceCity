#ifndef EC_GRAPH_H
#define EC_GRPAH_H

#include <ec_list.h>

void graph_list_add(s_list *list, int x, int y);
void graph_list_free_node(s_node *node);
s_vector2i graph_list_get_vector2i(s_node *node);
void graph_list_add_surrouding(s_list *list, s_vector2i v);

int ec_graph_supply_building(s_list *list, int *available, s_vector2i pos, s_ressource*(*get_resrc)(s_building *b));
int ec_graph_supply_board(s_building *b, s_vector2i pos, s_ressource*(*get_resrc)(s_building *b));

#endif
