/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ics.h"
#include "emalloc.h"
#include "listy.h"
#include <time.h>// 7/24 added

node_t *new_node(event_t *val) {
    assert( val != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    temp->val = val;
    temp->next = NULL;
    temp->previous = NULL;//7/25 added
    
    return temp;
}

void freeList(node_t *head)
{
    node_t *tmp;
    
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp->val);
        tmp->val =NULL;
        tmp->previous=NULL;
        tmp->next=NULL;
        free(tmp);
        tmp=NULL;
    }
    assert(head == NULL);
}

void apply(node_t *list, void (*fn)(node_t *list, void *, 
           char *yyyymmdd_from), void *arg, char *yyyymmdd_from, char *yyyymmdd_to)
{
    for (list = list->next ; list != NULL; list = list->next)
    {
         if(strncmp(yyyymmdd_from, list->val->dtstart, 8) <= 0 
            && strncmp(list->val->dtstart, yyyymmdd_to, 8) <= 0)
         {
             (*fn)(list, arg, yyyymmdd_from);
         }
    }
}

node_t *insert(node_t *temp_node, node_t *current, node_t *previous)
{
    
    while(current != NULL)
    {
        
        if( strncmp( current->val->dtstart , temp_node->val->dtstart, 16 ) < 0 )
        {
            current = current->next;
            previous = previous->next;
        }
        else if(strncmp( current->val->dtstart , temp_node->val->dtstart, 16 ) == 0
                && strncmp(current->val->summary ,temp_node->val->summary, SUMMARY_LEN) < 0 )
        {
            current = current->next;
            previous = previous->next;
        }
        else
        {
            break;
        }
    }
    /* insert */
    temp_node->next = current;
    previous->next  = temp_node;
    temp_node->previous = previous;
    if(current != NULL)
    {
        current->previous = temp_node;
    }
    
    return temp_node;
}








