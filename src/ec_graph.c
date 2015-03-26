
#include <stdlib.h>
#include <stdio.h>

#include <ec_list.h>
#include <ec_graph.h>

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

int main2(void)
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

