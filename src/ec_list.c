
#include <stdlib.h>
#include <stdio.h>

#include <ec_list.h>

s_list *list_new(void)
{
    s_list *ret = ec_utils_malloc(sizeof(s_list));
    ret->head = NULL;
    ret->tail = NULL;
    return ret;
}

void list_free(s_list *list)
{
    s_node *node = list->head;

    while (node != NULL)
    {
        s_node *tmp = node;
        node = node->next;

        free(tmp->payload);
        free(tmp);
    }

    free(list);
}

void list_add(s_list *list, void *payload)
{
    s_node *new = ec_utils_malloc(sizeof(s_node));

    new->payload = payload;
    new->next = NULL;

    if (list->head == NULL)  /* empty list */
    {
        list->head = new;
    }
    else if (list->tail == NULL) /* only one element */
    {
        list->head->next = new;
        list->tail = new;
    }
    else /* at least two elements */
    {
        list->tail->next = new;
        list->tail = new;
    }
}

void *list_pop(s_list *list)
{
    if (list->head == NULL) /* empty list */
    {
        return NULL;
    }
    else /* not empty list */
    {
        s_node *poped = NULL;

        poped = list->head;
        list->head = list->head->next;

        if (list->head == list->tail)
            list->tail = NULL;

        return poped;
    }
}
