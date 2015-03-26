
#include <stdlib.h>
#include <stdio.h>

#include <ec_list.h>

s_list *list_new(void)
{
    s_list *ret = malloc(sizeof(s_list));
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
    s_node *new = malloc(sizeof(s_node));

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

void list_print(s_list *list, char *id)
{
    printf("==> %s\n", id);

    if (list->head == NULL && list->tail == NULL)
    {
        printf("\tnull null\n");
    }
    else if (list->head != NULL)
    {
        s_node *node_cur = NULL;

        node_cur = list->head;
        do
        {
            printf("\t%p\t%.0f\n", (void*)node_cur, ((s_vector*)node_cur->payload)->x);
            node_cur = node_cur->next;
        }
        while (node_cur != NULL);
        printf("\n");
    }
    else
    {
        printf("Erreur inattendue ... %s %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}
