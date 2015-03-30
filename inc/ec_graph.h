#ifndef EC_GRAPH_H
#define EC_GRPAH_H

#include <ec_list.h>

void graph_list_add(s_list *list, int x, int y);
s_vector graph_list_ppaf(s_node *node);
void ec_graph_supply_serve(int x, int y);

#endif
