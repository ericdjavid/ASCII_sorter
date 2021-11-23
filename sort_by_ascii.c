
/**
 * C program ton sort a list of argument according to ASCII values 
 */

// TODO : deal with the prev pointer too
/* Particularite d une liste doublement chainee */
// -> le 1er element et le dernier element de la liste doivent etre NULL
// -> la liste de controle pointe sur ces 2 elements qui sont NULL

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define SUCCESS -1
#define FAILURE 0

typedef enum t_bool
{
    FALSE,
    TRUE
} s_bool;

typedef struct t_elem
{
    char *str;
    int index;
    struct t_elem *next;
    struct t_elem *prev;
} s_elem;

typedef struct t_control
{
    s_elem *first;
    s_elem *end;
    unsigned int size;
} s_control;

s_control *init()
{
    s_control *list = malloc(sizeof(*list));

    list->first = NULL;
    list->end = NULL;
    list->size = 0;
    return (list);
}

void add_index(s_elem *elem)
{
    int i = 0;
    while (elem->next)
    {
        elem->index = ++i;
        elem = elem->next;
    }
    elem->index = ++i;
    return;
}

/* Cas particulier, si l liste est vide, le premier elem de la list est aussi le dernier */
int first_alim(s_control *list, char *str)
{
    s_elem *new = malloc(sizeof(*new));
    if (!new)
        return FAILURE;
    new->index = 1;
    new->str = strdup(str);
    new->prev = list->first;
    new->next = list->end;
    list->size++;
    list->first = new;
    list->end = new;
}

int add_beg(s_control *list, char *str)
{
    s_elem *new = malloc(sizeof(*new));
    new->str = strdup(str);  // on copie la str ds new elem
    new->prev = NULL;        // l'elem precedant le new est NULL
    new->next = list->first; //le prochain est le 1er elem de la list
    list->first->prev = new; // le pointeur precedent du 1er elem pointe sur le nouvel elem
    new->index = 1;
    list->first = new; // le first de la liste de controle = le new elem
    list->size++;
}

void print_list(s_control *list)
{
    s_elem *elem;

    elem = list->first;
    while (elem != NULL)
    {
        printf("%d - %s\n", elem->index, elem->str);
        elem = elem->next;
    }
    printf("\n");
    return;
}

void free_list(s_control *list)
{
    s_elem *tmp;

    tmp = list->first;
    while (tmp)
    {
        //ORDER IS IMPORTANT
        list->first = list->first->next; //must be first or else you will free with temp and try to read
        free(tmp->str);
        free(tmp);
        tmp = list->first;
    }
    free(list);
}

int swap_elem(int pos1, int pos2, s_control *list)
{
    s_elem *tmp;
    s_elem *node1, *node2, *prev1, *prev2;
    int i = 0;

    prev1 = NULL;
    prev2 = NULL;
    node1 = NULL;
    node2 = NULL;
    tmp = list->first;

    if (pos1 == pos2)
        return SUCCESS;
    if (pos1 <= 0 || pos1 > list->size || pos2 <= 0 || pos2 > list->size)
        return FAILURE;

    // printf("-------------------\n");
    // printf("size of list is %d\n", list->size);
    while (tmp && ++i <= list->size)
    {
        if (tmp->index == pos1 - 1)
            prev1 = tmp;
        if (tmp->index == pos1)
            node1 = tmp;
        if (tmp->index == pos2 - 1)
            prev2 = tmp;
        if (tmp->index == pos2)
            node2 = tmp;
        tmp = tmp->next;
    }
    // Link previous of node1 with node2
    if (prev1 != NULL)
    {
        prev1->next = node2;
        // printf("prev node 1 is %s\n", prev1->str);
    }

    // Link previous of node2 with node1
    if (prev2 != NULL)
    {
        prev2->next = node1;
        // printf("prev node 2 is %s\n\n", prev2->str);
    }

    // printf("node 1 is %s\n", node1->str);
    // printf("node 2 is %s\n", node2->str);
    s_elem *tmp1;
    if (node2->next == NULL)
    {
        tmp1 = node1->next;
        node2->next = node1->next;
        node1->next = NULL;
    }
    else
    {
        tmp1 = node1->next;
        node1->next = node2->next;
        node2->next = tmp1;
    }
    if (prev1 == NULL)
        list->first = node2;
    else if (prev2 == NULL)
        list->first = node1;
    add_index(list->first);
    // print_list(list);
    // printf("-------------------\n");
    return SUCCESS;
}

s_bool is_ascii_ordered(char *str1, char *str2)
{
    int i = 0;

    //TODO: pbm avec bwieuu et bwieu mai I think que c est plutot li au swap
    while (str1[i] && str2[i])
    {
        while (str1[i] == str2[i])
            i++;
        if (str1[i] > str2[i])
            return FALSE;
        return TRUE;
    }
    if (!str2[i])
        return TRUE;
}

int order_ascii(s_control *list)
{
    s_elem *tmp;
    int i = -1;
    int j = -1;

    j = -1;
    tmp = list->first;

    while (1)
    {
        if (tmp == NULL || tmp->next == NULL)
            break;
        // printf("Comparing |%s| with |%s| \n", tmp->str, tmp->next->str);
        if (!is_ascii_ordered(tmp->str, tmp->next->str))
        {
            // printf("not ordered!\n");
            swap_elem(tmp->index, tmp->next->index, list);
        }
        tmp = tmp->next;
    }
    return SUCCESS;
}

s_bool check_order(s_control *list)
{
    s_elem *tmp;
    int i;

    i = -1;

    tmp = list->first;
    while (tmp->next)
    {
        if (is_ascii_ordered(tmp->str, tmp->next->str) == FALSE)
            return (FALSE);
        tmp = tmp->next;
    }
    // print_list(list);
    printf("List is ASCII ordered!\n");
    return (TRUE);
}

int main(int argc, char **argv)
{
    s_control *list;
    int i = 0;
    int j = 0;

    if (argc == 1)
        return (printf("invalid numb of arg\n"));
    /* Get strings in parms and put them in char *arr[] */
    char **arr = malloc(sizeof(char *) * argc);
    if (!arr)
        return (FAILURE);
    while (++i < argc)
        arr[i - 1] = ft_strdup(argv[i]);
    arr[i - 1] = NULL;

    /* init linked list */
    list = init();
    first_alim(list, arr[j]);
    while (arr[++j])
        add_beg(list, arr[j]);
    add_index(list->first);
    printf("Printed linked list : \n");
    print_list(list);

    while (check_order(list) == FALSE)
        order_ascii(list);

    printf("New printed linked list : \n");
    print_list(list);
    /* Free char *arr[] */
    j = -1;
    while (arr[++j])
        free(arr[j]);
    free(arr);
    free_list(list);
    return (SUCCESS);
}