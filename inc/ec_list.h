#ifndef EC_LIST_H
#define EC_LIST_H

#include <ec_allegro.h>

typedef struct      _s_node
{
    void            *payload;
    struct _s_node  *next;
}                   s_node;

typedef struct      _s_list
{
    s_node *head;
    s_node *tail;
}                   s_list;

s_list *list_new(void);
void list_free(s_list *list);
void list_add(s_list *list, void *payload);
void *list_pop(s_list *list);
void list_print(s_list *list, char *id);

#endif
